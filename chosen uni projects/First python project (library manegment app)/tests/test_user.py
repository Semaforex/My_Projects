from book import Book
from database import Database
import global_variables as gl
from pytest import raises
from datetime import date, timedelta
from errors import (
    EmptyPasswordError,
    EmptyUsernameError,
    BookNotAvailableError,
    TakenUsernameError,
    PasswordTooShortError,
    BookNotInPossetionError,
    NoExtentionsLeftError,
    BookReservedError
)
from user import Customer


def prepare():
    gl.database = Database()
    gl.database.clear()


def test_customer_init_auto():
    prepare()
    c = Customer('user', '12345678')
    assert c.username == 'user'
    assert c.password == '12345678'
    assert c.books == []
    assert c.history == []
    gl.database.clear()


def test_customer_init_manual():
    prepare()
    book1, book2 = Book('a', 'a'), Book('b', 'b')
    gl.database.add_book(book1, 'test.txt')
    gl.database.add_book(book2, 'test.txt')
    c = Customer()
    data = {
        'username': 'user',
        'data': {
            'password': '12345678',
            'books': [0, 1],
            'history': [('Borrowed A by A', '2003-11-11'), ('Borrowed B by B', '2003-11-21')],
            'activity': 2
            }
        }
    c.load_from_dic(data)
    assert c.username == 'user'
    assert c.password == '12345678'
    assert c.books == [book1, book2]
    assert c.history == [('Borrowed A by A', '2003-11-11'), ('Borrowed B by B', '2003-11-21')]
    assert c.activity == 2
    gl.database.clear()


def test_customer_init_errors():
    prepare()
    with raises(EmptyUsernameError):
        Customer('', 'lalalalalala')
    with raises(EmptyPasswordError):
        Customer('tralallili', '')
    with raises(PasswordTooShortError):
        Customer('user', 'lalal')
    gl.database.add_customer(Customer('taken', '12345678'))
    with raises(TakenUsernameError):
        Customer('taken', 'lalalalalala')
    gl.database.clear()


def test_borrow_return_book():
    prepare()
    book = Book('author', 'title')
    gl.database.add_book(book, 'test.txt')
    c = Customer('username', 'password')
    gl.database.add_customer(c)
    c.borrow_book(book)
    assert c.books == [book]
    c.return_book(book)
    assert c.books == []
    h = c.history
    assert h[0][0] == f'Borrowed {book.title} by {book.author}'
    assert h[1][0] == f'Returned {book.title} by {book.author}'
    gl.database.clear()


def test_boorrow_return_book_error():
    prepare()
    book1 = Book('a', 'a')
    gl.database.add_book(book1, 'test.txt')
    c1, c2 = Customer('u', 'password'), Customer('w', 'password')
    gl.database.add_customer(c1)
    gl.database.add_customer(c2)
    c2.borrow_book(book1)
    with raises(BookNotAvailableError):
        c1.borrow_book(book1)
    with raises(BookNotInPossetionError):
        c1.return_book(book1)
    gl.database.clear()


def test_extend_book():
    prepare()
    book1 = Book('a', 'a')
    gl.database.add_book(book1, 'test.txt')
    c1 = Customer('c', 'password')
    gl.database.add_customer(c1)
    c1.borrow_book(book1)
    c1.extend_return_date(book1)
    assert book1.extentions_left == 2
    c1.extend_return_date(book1)
    assert book1.extentions_left == 1
    c1.extend_return_date(book1)
    assert book1.extentions_left == 0
    assert book1.return_date == date.today()+timedelta(days=120)
    with raises(NoExtentionsLeftError):
        c1.extend_return_date(book1)
    c1.return_book(book1)
    assert book1.extentions_left == 3
    gl.database.clear()


def test_reserve_book():
    prepare()
    book1, book2 = Book('a', 'a'), Book('b', 'b')
    gl.database.add_book(book1, 'test.txt')
    c1 = Customer('c', 'password')
    c2 = Customer('c2', 'password')
    gl.database.add_customer(c1)
    gl.database.add_customer(c2)
    with raises(BookNotAvailableError):
        c1.reserve_book(book2)
    c2.borrow_book(book1)
    c1.reserve_book(book1)
    assert book1.reserved == 'c'
    assert not c2.reserve_book(book2)
    c2.return_book(book1)
    assert book1 in c1.books
    assert not book1.reserved
    c2.reserve_book(book1)
    assert book1.reserved == 'c2'
    gl.database.clear()


def test_check_if_in_possetion():
    prepare()
    book1, book2 = Book('a', 'a'), Book('b', 'b')
    gl.database.add_book(book1, 'test.txt')
    gl.database.add_book(book2, 'test.txt')
    c1 = Customer('c', 'password')
    gl.database.add_customer(c1)
    c1.borrow_book(book1)
    assert book1 == c1.check_if_in_possetion('a', 'a')
    assert not c1.check_if_in_possetion('b', 'b')
    c1.return_book(book1)
    assert not c1.check_if_in_possetion('a', 'a')
    gl.database.clear()


def test_extend_when_reserved():
    prepare()
    book1 = Book('a', 'a')
    c1 = Customer('c', 'password')
    c2 = Customer('d', 'password')
    gl.database.add_book(book1, 'test.txt')
    gl.database.add_customer(c1)
    gl.database.add_customer(c2)
    c1.borrow_book(book1)
    c2.reserve_book(book1)
    with raises(BookReservedError):
        c1.extend_return_date(book1)
    gl.database.clear()
