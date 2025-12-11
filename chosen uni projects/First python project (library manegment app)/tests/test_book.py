from book import Book
from database import Database
import global_variables as gl
from datetime import date, timedelta
from pytest import raises
import errors as er


def prepare():
    gl.database = Database()
    gl.database.clear()


def test_book_init_auto():
    prepare()
    book1 = Book('nAme_surName', 'wOrd1_word2_wOrd3')
    assert book1.author == 'Name Surname'
    assert book1.title == 'Word1 Word2 Word3'
    assert book1.time_limit == 30
    assert book1.id == 0
    assert not book1.return_date
    assert not book1.reserved
    assert book1.extentions_left == 3
    gl.database.clear()


def test_book_init_given():
    prepare()
    book1 = Book()
    book1.load_from_dic({
                         'id': 32,
                         'data': {
                             'title': 'Title',
                             'author': 'Author1',
                             'reserved': True,
                             'time_limit': 100,
                             'extentions_left': 1,
                             'popularity': 10,
                             'return_date': str(date.today())
                             },
                         'body': 'not_important'
                         })
    assert book1.author == 'Author1'
    assert book1.title == 'Title'
    assert book1.time_limit == 100
    assert book1.id == 32
    assert book1.return_date == date.today()
    assert book1.reserved
    assert book1.extentions_left == 1
    assert book1.popularity == 10
    gl.database.clear()


def test_book_errors():
    prepare()
    with raises(er.EmptyTitleError):
        Book('author', '')
    with raises(er.EmptyAuthorError):
        Book('', 'lala')
    with raises(er.NegativeTimeLimitError):
        Book('author', 'title', -10)
    gl.database.clear()


def test_set_date_of_return():
    prepare()
    book = Book('author', 'title', 100)
    book.set_date_of_return(False)
    assert book.return_date == date.today()+timedelta(days=100)
    book.set_date_of_return(True)
    assert not book.return_date
    book.set_date_of_return(False)
    assert book.return_date == date.today()+timedelta(days=100)
    book.set_date_of_return(False)
    assert book.return_date == date.today()+timedelta(days=130)
    gl.database.clear()


def test_set_extentions():
    prepare()
    book = Book('author', 'title')
    with raises(er.NegativeExtentionsLeftError):
        book.set_extentions(-1)
    book.set_extentions(10)
    assert book.extentions_left == 10
    gl.database.clear()


def test_set_reserved():
    prepare()
    book = Book('author', 'title')
    book.set_reserved('tralalala')
    assert book.reserved == 'tralalala'
    gl.database.clear()


def test_load_from_dic():
    prepare()
    book = Book()
    dic = {
         'id': 32,
         'data': {
             'title': 'Title',
             'author': 'Author1',
             'reserved': 'user1',
             'time_limit': 100,
             'extentions_left': 1,
             'popularity': 10,
             'return_date': str(date(2024, 1, 1))
             },
         'body': 'not_important'
         }
    book.load_from_dic(dic)
    assert book.id == 32
    assert book.title == 'Title'
    assert book.author == 'Author1'
    assert book.time_limit == 100
    assert book.extentions_left == 1
    assert book.popularity == 10
    assert book.return_date == date(2024, 1, 1)
    assert book.reserved == 'user1'
    gl.database.clear()


def test_save_into_dic():
    prepare()
    book = Book('author1', 'title1', 128)
    gl.database.add_book(book, 'test.txt')
    book.set_date_of_return()
    book.set_reserved('user1')
    dic = {
        'author': 'Author1',
        'title': 'Title1',
        'time_limit': 128,
        'extentions_left': 3,
        'popularity': 1,  # because setting date of return increases popularity
        'return_date': str(date.today()+timedelta(days=128)),
        'reserved': 'user1'
    }
    data = {'data': dic, 'id': 0}
    assert data == book.save_into_dic()


def test_book_repr1():
    prepare()
    book = Book()
    dic = {
         'id': 32,
         'data': {
             'title': 'Title',
             'author': 'Author1',
             'reserved': 'user1',
             'time_limit': 100,
             'extentions_left': 1,
             'popularity': 10,
             'return_date': str(date(2024, 1, 1))
             },
         'body': 'not_important'
         }
    book.load_from_dic(dic)
    s = '''    Author: Author1
    Title: Title
    max time of holding: 100 days
    Extentions left: 1
    Return date: 2024-01-01(PAST DUE!!)
    reserved by user1
'''
    assert s == repr(book)
    gl.database.clear()
