from book import Book
from datetime import date
import global_variables as gl
from strings_and_messages import format_string
from errors import (
    BookNotAvailableError,
    BookNotInPossetionError,
    NoExtentionsLeftError,
    UserDoesNotExistError,
    BookReservedError,
    check_username,
    check_password,
)


class User:
    """
    A class used to represent a user
    (either a customer or a librarian)

    ...

    Attributes
    ----------
    _username : str
        user's username
    _password : str
        user's password
    """
    def __init__(self, username: str, password: str):
        check_username(username)
        self._username = username
        check_password(password)
        self._password = password

    @property
    def username(self) -> str:
        return self._username

    @property
    def password(self) -> str:
        return self._password

    def change_name(self, new_name: str):
        """used to change username"""
        check_username(new_name)
        self._username = new_name

    def set_password(self, password: str):
        """used to set password"""
        check_password(password)
        self._password = password

    def check_password_correct(self, password: str) -> bool:
        """checks if password is correct"""
        if password == self._password:
            return True
        return False


class Customer(User):
    """
    A class used to represent a customer

    ...

    Attributes
    ----------
    _books : list[Book]
        list of books currently in user's possetion
    _history : list[str]
        list of logs representing user's actions
    _activity : int
        number representing how many actions did the user perform
    """

    def __init__(self, username: str = 'temp_username', password: str = 'temp_password'):
        super().__init__(username, password)
        self._history = []
        self._activity = 0
        self._books = []

    @property
    def books(self) -> list[Book]:
        return self._books

    @property
    def history(self) -> list[(str, str)]:
        return self._history

    @property
    def activity(self) -> int:
        return self._activity

    def load_from_dic(self, data: dict[str: str | dict[str: list[int] | int | str]]):
        """loads data from dictionary into customer parameters"""
        username = data['username']
        dic = data['data']
        self._username = username
        self._password = dic['password']
        self._history = dic['history']
        books_id = dic['books']
        self._activity = dic['activity']
        self._books = [book for book in gl.database.available_books if book.id in books_id]

    def save_into_dic(self) -> dict[str: str | dict[str: list[int] | int | str]]:
        """saves customer parameters into dict format to send to the database"""
        data = {'username': self._username}
        dic = {}
        dic['password'] = self._password
        dic['history'] = self._history
        dic['activity'] = self._activity
        books_id = [book.id for book in self._books]
        dic['books'] = books_id
        data['data'] = dic
        return data

    def borrow_book(self, book: Book):
        """adds a book to _books attribute"""
        if book.return_date:
            raise BookNotAvailableError
        book.set_date_of_return()
        self._books.append(book)
        self.add_history(f'Borrowed {book.title} by {book.author}')
        gl.database.update_customer(self)
        gl.database.update_book(book)

    def return_book(self, book: Book):
        """removes a book from _books attribute"""
        if book not in self._books:
            raise BookNotInPossetionError
        self._books.remove(book)
        book.set_date_of_return(True)
        book.set_extentions(3)
        if book.reserved:
            try:
                next_user = gl.database.user_with_username(book.reserved)
                book.set_reserved()
                next_user.borrow_book(book)
            except UserDoesNotExistError:
                pass
        self.add_history(f'Returned {book.title} by {book.author}')
        book.set_reserved()
        gl.database.update_book(book)
        gl.database.update_customer(self)

    def reserve_book(self, book: Book) -> bool:
        """reserves a book. Returns True if succesfull,
        otherwise false"""
        if book.reserved:
            return False
        book.set_reserved(self.username)
        self.add_history(f'Reserved {book.title} by {book.author}')
        gl.database.update_book(book)
        gl.database.update_customer(self)
        return True

    def extend_return_date(self, book: Book):
        """extends books return date"""
        if book.extentions_left == 0:
            raise NoExtentionsLeftError
        if book.reserved:
            raise BookReservedError
        book.set_date_of_return()
        book.set_extentions(book.extentions_left-1)
        self.add_history(f'Extended {book.title} by {book.author}')
        gl.database.update_customer(self)

    def check_if_in_possetion(self, author: str, title: str) -> Book:
        """returns book that matches author and title provided in the
        arguments. If there is none, returns None"""
        author = format_string(author)
        title = format_string(title)
        for book in self.books:
            if author == book.author and title == book.title:
                return book
        return None

    def add_history(self, log: str):
        """adds a log to _history attribute"""
        self._activity += 1
        self._history.append((log, str(date.today())))

    def __str__(self) -> str:
        return f'{self._username}. Currently {len(self._books)} in possession.'

    def __repr__(self):
        s = f'\nUsername: {self.username}\n'
        s += 'Books in possetion:\n'
        if not self.books:
            s += '  >No books in possetion\n'
        else:
            s += '-'*20
            for i, book in enumerate(self.books):
                s += f'\n{repr(book)}'
            s += '-'*20+'\n\n'
        s += 'Reservations:\n'
        res = [book for book in gl.database.available_books if book.reserved == self.username]
        if not res:
            s += '  >No reservations made\n\n'
        else:
            s += '-'*20
            for i, book in enumerate(res):
                s += f'\n{repr(book)}'
            s += '-'*20+'\n\n'
        s += 'History:\n'
        if not self._history:
            s += '  >No history yet\n'
        else:
            s += '-'*20+'\n'
            for log, d in self._history:
                s += f'  {log} ({d})\n'
            s += '-'*20+'\n'
        return s+'\n'


class Librarian(User):
    """
    A class used to represent a librarian
    """
    def __init__(self, username: str = 'temp_username', password: str = 'temp_password'):
        super().__init__(username, password)

    def load_from_dic(self, data: dict[str: str | dict[str: str]]):
        """loads data from dictionary into librarian parameters"""
        username = data['username']
        dic = data['data']
        password = dic['password']
        self._username = username
        self._password = password    

    def save_into_dic(self) -> dict[str: str | dict[str: list[int] | int | str]]:
        """saves librarian parameters into dict format to send to the database"""
        data = {'username': self._username}
        dic = {}
        dic['password'] = self._password
        data['data'] = dic
        return data
