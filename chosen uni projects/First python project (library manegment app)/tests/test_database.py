from book import Book
from database import Database
import global_variables as gl
from pytest import raises
import errors as er
from user import Customer, Librarian
import os
import shutil


def load_test_data():
    for filename in os.listdir('data/'):
        os.remove(os.path.join('data/', filename))
    origin = 'tests/data_for_testing/'
    target = 'data/'
    files = os.listdir(origin)
    for file_name in files:
        shutil.copy(origin+file_name, target+file_name)


def prepare():
    load_test_data()
    gl.database = Database()
    gl.database.setup()


def test_database_setup():
    prepare()
    assert len(gl.database.customer_list) == 3
    assert len(gl.database.librarian_list) == 2
    assert len(gl.database.available_books) == 4
    assert gl.database.user_list == gl.database.customer_list+gl.database.librarian_list
    gl.database.clear()


def test_clear():
    prepare()
    gl.database.clear()
    assert not gl.database.available_books
    assert not gl.database.customer_list
    assert not gl.database.librarian_list
    with open(gl.BOOKS_PATH, 'r') as b:
        assert b.read() == ''
    with open(gl.CUSTOMERS_PATH, 'r') as c:
        assert c.read() == ''
    with open(gl.LIBRARIANS_PATH, 'r') as L:
        assert L.read() == ''


def test_add_customer():
    prepare()
    c1 = Customer('customer10', 'password')
    gl.database.add_customer(c1)
    assert c1 in gl.database.customer_list
    gl.database.clear()


def test_add_customer_error():
    prepare()
    c1 = gl.database.customer_list[0]
    with raises(er.UserAlreadyInDatabaseError):
        gl.database.add_customer(c1)


def test_add_librarian():
    prepare()
    l1 = Librarian('librarian10', 'password')
    gl.database.add_librarian(l1)
    assert l1 in gl.database.librarian_list
    gl.database.clear()


def test_add_librarian_error():
    prepare()
    l1 = gl.database.librarian_list[0]
    with raises(er.UserAlreadyInDatabaseError):
        gl.database.add_librarian(l1)
    gl.database.clear()


def test_add_book():
    prepare()
    b1 = Book('autho10', 'book10')
    gl.database.add_book(b1, 'test.txt')
    assert b1 in gl.database.available_books
    gl.database.clear()


def test_add_book_error():
    prepare()
    b1 = gl.database.available_books[0]
    with raises(er.BookAlreadyInDatabaseError):
        gl.database.add_book(b1, 'test.txt')
    errorBook = Book('author11', 'title11')
    with raises(er.BookFileNotHere):
        gl.database.add_book(errorBook, 'adjasj')
    gl.database.clear()


def test_delete_book():
    prepare()
    b1 = gl.database.available_books[1]
    gl.database.delete([b1.author, b1.title])
    assert b1.id not in [book.id for book in gl.database.available_books]
    gl.database.clear()


def test_delete_customer():
    prepare()
    c1 = gl.database.customer_list[0]
    gl.database.delete([c1.username])
    assert c1 not in gl.database.customer_list
    gl.database.clear()


def test_delete_user_error():
    prepare()
    with raises(er.UserDoesNotExistError):
        gl.database.delete(['non_existant_user'])
    gl.database.clear()


def test_user_with_username():
    prepare()
    c1 = gl.database.customer_list[0]
    assert gl.database.user_with_username(c1.username) == c1
    gl.database.clear()


def test_user_with_username_error():
    prepare()
    with raises(er.UserDoesNotExistError):
        gl.database.user_with_username('user_not_existant')


def test_check_if_book_in_database():
    prepare()
    list = gl.database.check_if_book_in_database('authOr1', '__Book1')
    assert len(list) == 2
    list = gl.database.check_if_book_in_database('not_here', 'not_here')
    assert not list
    gl.database.clear()


def test_check_if_available():
    prepare()
    c1 = gl.database.customer_list[0]
    b1 = gl.database.available_books[1]
    assert b1 == gl.database.check_if_available('author2', 'book2')
    c1.borrow_book(b1)
    assert not gl.database.check_if_available('author2', 'book2')
    gl.database.clear()
