from user import Librarian, Customer, User
from book import Book
import os
import copy
from errors import (
    UserAlreadyInDatabaseError,
    BookAlreadyInDatabaseError,
    BookNotAvailableError,
    UserDoesNotExistError,
    BookFileNotHere
)
import json
import global_variables as gl
from strings_and_messages import format_string
from strings_and_messages import fail_message


class Database():
    """
    A class used to store and manage data and
    operate on json files in the data folder

    ...

    Attributes
    ----------
    _customers_path : str
        path to customers.json file
    _librarians_path : str
        path to librarians.json file
    _books_path : str
        path to books.json file
    _customer_list : list[Customer]
        list of all customers registered in the library
    _librarian_list : list[Librarian]
        list of all librarians registered in the library
    _available_books : list[Book]
        list of all books registered in the library
    """
    def __init__(self):
        self._customers_path = gl.CUSTOMERS_PATH
        self._librarians_path = gl.LIBRARIANS_PATH
        self._books_path = gl.BOOKS_PATH
        self._customer_list = []
        self._librarian_list = []
        self._available_books = []

    @property
    def customer_list(self):
        return self._customer_list

    @property
    def librarian_list(self):
        return self._librarian_list

    @property
    def user_list(self):
        return self._customer_list+self._librarian_list

    @property
    def available_books(self):
        return self._available_books

    def setup(self):
        """loads data stored in the data folder into the database"""
        List = []
        if os.path.isfile(self._books_path):
            with open(self._books_path, 'r') as b:
                try:
                    List = json.load(b)
                except json.decoder.JSONDecodeError:
                    pass
        for data in List:
            book = Book()
            book.load_from_dic(data)
            self._available_books.append(book)
        List = []
        if os.path.isfile(self._customers_path):
            with open(self._customers_path, 'r') as C:
                try:
                    List = json.load(C)
                except json.decoder.JSONDecodeError:
                    pass
        for data in List:
            new_customer = Customer()
            new_customer.load_from_dic(data)
            self._customer_list.append(new_customer)
        List = []
        if os.path.isfile(self._librarians_path):
            with open(self._librarians_path, 'r') as L:
                try:
                    List = json.load(L)
                except json.decoder.JSONDecodeError:
                    pass
        for data in List:
            new_librarian = Librarian()
            new_librarian.load_from_dic(data)
            self._librarian_list.append(new_librarian)

    def add_customer(self, customer: Customer):
        """adds a customer to the database"""
        if customer in self.user_list:
            raise UserAlreadyInDatabaseError
        List = []
        if os.path.isfile(self._customers_path):
            with open(self._customers_path, 'r') as C:
                try:
                    List = json.load(C)
                except json.decoder.JSONDecodeError:
                    pass
        List.append(customer.save_into_dic())
        with open(self._customers_path, 'w') as C:
            json.dump(List, C, indent=6)

        self._customer_list.append(customer)

    def add_librarian(self, librarian: Librarian):
        """adds a librarian to the database"""
        if librarian in self.user_list:
            raise UserAlreadyInDatabaseError
        self._librarian_list.append(librarian)
        List = []
        if os.path.isfile(self._librarians_path):
            with open(self._librarians_path, 'r') as L:
                try:
                    List = json.load(L)
                except json.decoder.JSONDecodeError:
                    pass
        List.append(librarian.save_into_dic())
        with open(self._librarians_path, 'w') as L:
            json.dump(List, L, indent=4)

    def add_book(self, book: Book, body_file: str):
        """adds a book into the database"""
        if book in self._available_books:
            raise BookAlreadyInDatabaseError
        Path = f'Put_Books_Here/{body_file}'
        try:
            with open(Path, 'r') as fh:
                body = fh.read()
        except FileNotFoundError:
            raise BookFileNotHere(body_file)
        self._available_books.append(book)
        data = book.save_into_dic()
        data['body'] = body
        List = []
        if os.path.isfile(self._books_path):
            with open(self._books_path, 'r') as b:
                try:
                    List = json.load(b)
                except json.decoder.JSONDecodeError:
                    pass
        List.append(data)
        with open(self._books_path, 'w') as b:
            json.dump(List, b, indent=9)

    def delete(self, words: list[str]) -> str:
        """Decodes delete command, deletes either a customer
        or a book, returns message regarding sucess/failure"""
        if len(words) == 1:
            user = self.user_with_username(words[0])
            if type(user) is Librarian:
                return fail_message[5]
            books_copy = copy.copy(user.books)
            for book in books_copy:
                user.return_book(book)
            self.remove_account(user)
            return f"Succesfully removed {words[0]}\n"
        else:
            books = self.check_if_book_in_database(words[0], words[1])
            if not books:
                return fail_message[6]
            b = None
            for book in books:
                if not book.return_date:
                    b = book
            if b:
                self.remove_book(b)
                return f'Succesfully removed {words[1]} by {words[0]}\n'
            else:
                return fail_message[7]

    def remove_book(self, book: Book):
        """removes a book from the database if it's available"""
        if book not in self._available_books:
            raise BookNotAvailableError
        self._available_books.remove(book)
        with open(self._books_path, 'r') as b:
            try:
                List = json.load(b)
            except json.decoder.JSONDecodeError:
                raise BookNotAvailableError
        for data in List:
            if data['id'] == book.id:
                List.remove(data)
                break
        with open(self._books_path, 'w') as b:
            json.dump(List, b, indent=9)

    def remove_account(self, user: User):
        """removes either a customer or a librarian from the database"""
        if user in self._customer_list:
            with open(self._customers_path, 'r') as C:
                List = json.load(C)
        elif user in self._librarian_list:
            with open(self._librarians_path, 'r') as L:
                List = json.load(L)
        else:
            raise UserDoesNotExistError
        for data in List:
            if data['username'] == user.username:
                List.remove(data)
        if user in self._customer_list:
            for book in self._available_books:
                if book.reserved == user.username:
                    book.set_reserved()
                    self.update_book(book)
            self._customer_list.remove(user)
            with open(self._customers_path, 'w') as C:
                json.dump(List, C, indent=5)
        elif user in self._librarian_list:
            self.librarian_list.remove(user)
            with open(self._librarians_path, 'w') as L:
                json.dump(List, L, indent=4)

    def user_with_username(self, username: str) -> User:
        """returns user object based on the given username
        if available in the database"""
        for user in self.user_list:
            if user.username == username:
                return user
        raise UserDoesNotExistError

    def update_book(self, book: Book):
        """updates the json file that contains info about this book"""
        if book not in self.available_books:
            raise BookNotAvailableError
        with open(self._books_path, 'r') as B:
            List = json.load(B)
        for data in List:
            if data['id'] == book.id:
                body = data['body']
                newdata = book.save_into_dic()
                newdata['body'] = body
                List.remove(data)
                List.append(newdata)
                break
        with open(self._books_path, 'w') as B:
            json.dump(List, B, indent=9)

    def update_customer(self, customer: Customer):
        """updates this customers dictionary in
        customers.json file"""
        if customer not in self.customer_list:
            raise UserDoesNotExistError
        with open(self._customers_path, 'r') as C:
            List = json.load(C)
        for data in List:
            if data["username"] == customer.username:
                new_data = customer.save_into_dic()
                List.remove(data)
                List.append(new_data)
                break
        with open(self._customers_path, 'w') as C:
            json.dump(List, C, indent=6)

    def check_if_book_in_database(self, author: str, title: str) -> list[Book]:
        """returns a list of books from the database that match the author
        and the title provided by arguments"""
        author = format_string(author)
        title = format_string(title)
        result = []
        for book in self.available_books:
            if author == book.author and title == book.title:
                result.append(book)
        if len(result) == 0:
            return None
        return result

    def check_if_available(self, author: str, title: str) -> Book:
        """returns any book that matches the author and the title
        from the arguments and is not currently in possetion
        of another user"""
        books = self.check_if_book_in_database(author, title)
        if not books:
            return None
        for book in books:
            if not book.return_date:
                return book
        return None

    def clear(self):
        """clears the entire database including the json files"""
        open(self._books_path, 'w')
        open(self._customers_path, 'w')
        open(self._librarians_path, 'w')
        self._available_books = []
        self._customer_list = []
        self._librarian_list = []
