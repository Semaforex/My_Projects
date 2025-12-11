from datetime import date, timedelta
import global_variables as gl
from strings_and_messages import format_string
from errors import (
    EmptyAuthorError,
    EmptyTitleError,
    NegativeExtentionsLeftError,
    NegativeTimeLimitError
)


class Book:
    """
    A class used to represent a book

    ...

    Attributes
    ----------
    _author : str
        book's author
    _title : str
        book's title
    _time_limt : int
        specifies for how long (in days) can the book be borrowed
        (by deafult 30)
    _id : int
        book's id. Unique to every book
    _return_date : date
        specifies a date by which the book should be returned.
        (If book is not in any customers posetion, _return_date = None)
    _reserved : str
        stores a username of the user that reserved this book.
        (If there is none, _reserved = None)
    _extentions_left: int
        specifies how many times more can the return date be postponed
        (at the beginning _extentions_left = 3)
    _popularity: int
        ammount of times the book has been borrowed
    """
    def __init__(self, Author: str = 'temp', Title: str = 'temp', time_limit: int = 30):
        if not Title:
            raise EmptyTitleError
        if not Author:
            raise EmptyAuthorError
        if time_limit <= 0:
            raise NegativeTimeLimitError
        self._author = format_string(Author)
        self._title = format_string(Title)
        self._time_limit = time_limit
        self._id = len(gl.database.available_books)
        self._reserved = None
        self._extentions_left = 3
        self._popularity = 0
        self._return_date = None

    @property
    def title(self) -> str:
        return self._title

    @property
    def author(self) -> str:
        return self._author

    @property
    def time_limit(self) -> int:
        return self._time_limit

    @property
    def return_date(self) -> date:
        return self._return_date

    @property
    def id(self) -> int:
        return self._id

    @property
    def reserved(self) -> bool:
        return self._reserved

    @property
    def extentions_left(self):
        return self._extentions_left

    @property
    def popularity(self):
        return self._popularity

    def load_from_dic(self, data: dict[str: dict[str: str | int] | int | str]):
        """loads data forwarded from the Database into parameters of the book object"""
        dic = data['data']
        id = data['id']
        self._title = dic['title']
        self._author = dic['author']
        self._id = id
        self._title = dic['title']
        self._time_limit = dic['time_limit']
        self._reserved = dic["reserved"]
        self._extentions_left = dic['extentions_left']
        self._popularity = dic['popularity']
        if dic["return_date"]:
            date_int = [int(s) for s in dic["return_date"].split('-')]
            self._return_date = date(date_int[0], date_int[1], date_int[2])
        else:
            self._return_date = None

    def save_into_dic(self) -> dict[str: dict[str: int | str] | str | int]:
        """saves parameters into dict format to send to the database"""
        dic = {}
        dic['author'] = self._author
        dic['title'] = self._title
        dic['time_limit'] = self._time_limit
        dic['reserved'] = self._reserved
        dic['extentions_left'] = self._extentions_left
        if self._return_date:
            dic['return_date'] = self._return_date.strftime('%Y-%m-%d')
        else:
            dic['return_date'] = None
        dic['popularity'] = self._popularity
        return {'id': self._id, 'data': dic}

    def set_date_of_return(self, reset: bool = False):
        """
        -if reset argument is true:
            removes book's return date.
        -else:
            -if there is no self._return_date:
                     sets a new return date based on _time_limit
                     and increases _popularity
            -else:
                     delays the _return_date by 30
        """
        if not reset:
            if not self._return_date:
                self._popularity += 1
                self._return_date = date.today() + timedelta(days=self._time_limit)
            else:
                self._return_date += timedelta(days=30)
        else:
            self._return_date = None

    def set_extentions(self, extentions_left: int):
        """sets _extentions_left to a given non-negative value"""
        if extentions_left < 0:
            raise NegativeExtentionsLeftError
        self._extentions_left = extentions_left

    def set_reserved(self, reserver: str = None):
        """changes which user currently reserves the book"""
        self._reserved = reserver

    def __repr__(self) -> str:
        s = f'    Author: {self.author}\n'
        s += f'    Title: {self.title}\n'
        s += f'    max time of holding: {self.time_limit} days\n'
        s += f'    Extentions left: {self._extentions_left}\n'
        if not self.return_date:
            s += '    In Stock'
        else:
            s += f'    Return date: {self.return_date}'
            if date.today() > self.return_date:
                s += '(PAST DUE!!)'
        s += '\n'
        if self.reserved:
            s += f'    reserved by {self.reserved}\n'
        return s
