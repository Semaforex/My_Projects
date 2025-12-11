# Library management application
## Table of contents
* [General info](#general-info)
* [Usage](#usage)
* [How does the app work](#how-does-the-app-work)
* [Additional information](#additional-information)

## General info
This Application simmulates workflow of a library. It stores the library data in .json files and allows for the following operations:
* creating librarian accounts (passhprase needed) \
[by deafult the passphrase is 'library123' and can be changed in global_variables.py file] 
* creating customer accounts 
### As a librarian:
* adding and removing books from the database
* removing customer accounts
* showing basic information about customer accounts
* searching for books
* removing your account
* showing statistics regarding books and customers
### As a customer:
* borrowing and returning books
* reserving books
* extending the return date of books for up to 3 times
* removing your account
* searching for books
* showing information about your account
###

## How does the app work
The application stores books in in `/data/books.json` file.
Note that the application stores contents of the books but it does not allow for displaying those contents as that is not it's purpose. It's functionality is limited to managing information about where the books are, for how long, when should they be returned etc. \
The app stores customer and librarian info in `/data/customers.json` and `/data/librarians.json` files respectively. \
\
The database object of class Database manages those files. It updates them after every change to either a customer, a book or a librarian. It also loads up data stored in those files after the program is activated. \
Both the Customer and the Librarian classes inherit from the User class. They are used to manage info about librarians (password, username) and customers (username, password, books borrowed, activity, etc.). \
The Book class manages information about books in the library such as: title, author, return date, etc. \
\
The app takes commands through the terminal. In the interface.py module those commands are interpreted, the appropriate logic is performed and the output is printed back onto the terminal.
## Usage
### Start
To begin using the library, run the 'main.py' file. You should see the following:
```
Welcome to the Library. Please choose one of the following commands:
login | create new account | help | exit [L/C/H/E]
```
We recommend running the `CLEAR` command before using the app.
### Creating an account
To create an account, type in either `C` or `create`. You will then be asked to choose the type of account to be created.
Type in either `C`/`customer` or `L`/`librarian`.
In the second case you will then be asked for the passphrase.
You will then be asked to choose the username and the password. The username must be unique for each user and the password must consist of at least 8 characters. Neither the password nor the username can contain a space.
```
Welcome to the Library. Please choose one of the following commands:
login | create new account | help | exit [L/C/H/E]
>C
Are you a librerian or a customer? [L/C]
>L
To create a librerian account you need to type in the Passphrase:
>library123
Pick a username
>librarian1
Pick a password
>password1
Welcome to the Library librarian1!
(Type "help" for available options)
What would you like to do?
>
```
### Logging in
After you create an account, you are automatically logged into that account. If you instead wish to log into an account from the starting menu type in either `L` or `login`. You will then be asked for the username and the password. want to to abort logging in, type in `!E`.
### Commands (librarian)
Commands presented below are available for use after logging into a librarian account.
#### 1) add
In order to add a book to the library you need to put a .txt file containing the body of a book in the Put_Books_Here directory located in the main folder. You then need to type in
```
>add [author] [title] [body_file] (max. time of holding) 
```
`[author]`, `[title]` and `[body_file]` arguments are obligatory and cannot contain any spaces. To separate words in the title and in the author arguments, use '_'. The `(max. time of holding)` argument is optional and specifies the maximum time that the book can be borrowed for (without extending the return date). The value given here must be a positive integer. By deafult this value will be equal to 30. \
Example: (Note that by deafult there is a test.txt file in Put_Books_Here)
```
Welcome to the Library librarian1!
(Type "help" for available options)
What would you like to do?
>add test_author test_book test.txt 40
Successfully added the book
```
There will then be a new file created in the `/data/books` folder named {i}.json where i+1 is the number of books already in the database. The file should look like that:
```
{
          "id": i,
          "data": {
                   "author": "Test Author",
                   "title": "Test Book",
                   "time_limit": 40,
                   "reserved": null,
                   "extentions_left": 3,
                   "return_date": null,
                   "popularity": 0
          },
          "body": "for testing"
 }
```
Note that you can add multiple books with the same author and title.
#### 2) delete
The `delete` command allows librarians to delete books and customers from the database. \
If you want to delete a user, type in `delete [username]`. \
If you want to delete a book, type in `delte [author] [title]`. \
(seperate words in author and title using '_') \
(Deleting a customer will automatically return all their books. \
 You cannot delete a book that's currently borrowed by a customer)
#### 3) info
The `info` command allows a librarian to get the inforamtions about a choosen customer. To acces the information simply type in `info [username]`. Example:
```
What would you like to do?
>info customer1

Username: customer1
Books in possetion:
--------------------
    Author: Test Author
    Title: Test Book
    max time of holding: 40 days
    Extentions left: 3
    Return date: 2024-02-11
--------------------

Reservations:
--------------------
    Author: Aut
    Title: New
    max time of holding: 30 days
    Extentions left: 3
    Return date: 2024-01-30
    reserved
--------------------

History:
--------------------
  Borrowed Test Book by Test Author (2024-01-02)
  Reserved New by Aut (2024-01-02)
--------------------
```
#### 4) activity
The `activity` command allows a librarian to display a bar chart representing the activity of each customer registered in the database. Activity point is given to each customer after sucesfully:
1) borrowing a book
2) returning a book
3) postponing the return date of a book
4) reserving a book \
\
To continue using the application after displaying the chart, You need to close it by clicking the 'x' in the corner
#### 5) popularity
The `popularity` command allows a librarian to display a pie chart representing the popularity of each book in the library. (Books with the same author and title are seen as one on this chart.) The more times a given book has been borrowed, the bigger part of the chart it occupies. 

To continue using the application after displaying the chart, You need to close it by clicking the 'x' in the corner
### Commands (customer)
Commands presented below are available for use after logging into a customer account.
#### 1) borrow
The `borrow` command allows a customer to borrow a book previously added to the library by a librarian if that book is not currently borrowed by someone else. You can only have one book with a given title and author at the same time \
 To borrow a book type in `borrow [author] [title]`.
(seperate words in author and title using '_') \
Example:
```
What would you like to do?
>borrow test_author test_book 
Successfully borrowed Test Book by Test Author
```
#### 2) reserve
The `reserve` command allows a customer to reserve a book previously added to the library by a librarian if that book is currently borrowed by someone else and is not reserved by anyone. You can only reserve one book with a given title and author at the same time \
 To reserve a book type in `reserve [author] [title]`.
(seperate words in author and title using '_')
#### 3) return
The `return` command allows a customer to return a book previously added to the library by a librarian if that book is in their possetion. \
 To return a book type in `return [author] [title]`.
(seperate words in author and title using '_') \
#### 5) extend
The `extend` command allows to extend the return date of a book for aditional 30 days for up to 3 times. To postpone a book's return date type in `extend [author] [title]`.
(seperate words in author and title using '_')
#### 4) myinfo
The `myinfo` command allows a customer to acces information about their account. To use it simply type `myinfo`.
### Commands (customer and librarian)
Commands presented below are available for use after logging into a customer or a librarian account.
#### 1) delacc
The `delacc` command deletes the account of a user that has typed it in. The application requires you to verify your idenity by typing in the password and to confirm your choise. Example:
 ```
 Welcome to the Library customer1!
(Type "help" for available options)
What would you like to do?
>delacc
Please type in your password
>password
Are you sure you want to delete your account?
>yes
Welcome to the Library. Please choose one of the following commands:
login | create new account | help | exit [L/C/H/E]
>
 ```
 After sucesfull deletion of a customer account, all books currently in his possetion will be returned.
#### 2) search
The `search` command allows you to search for a book in a library by providing fragments of the title or the author name(s). You can give as much fragments as you wish (separate them using space). \
`search [fragment 1] [fragment 2] ... [fragment n]` \
If you wish to display all books from the library, simply type in `search` without providing any arguments. Example:
```
>search 3 inie
    Author: Michal Adam Paradowski
    Title: Dlugi Tytul 3 Slowa
    max time of holding: 30 days
    Extentions left: 3
    In Stock

    Author: Sokrates
    Title: Iniemamocni Sa Fajni
    max time of holding: 30 days
    Extentions left: 3
    In Stock

What would you like to do?
>search iniemamocni     
    Author: Sokrates
    Title: Iniemamocni Sa Fajni
    max time of holding: 30 days
    Extentions left: 3
    In Stock
```
## Additional Information
1) If you wish to run tests from tests directory, note that this will clear the database before testing.
2) Book's contents are stored in .json files but are not accesible through the application since that's not the purpose of the app.
3) To clear the database type in `Clear` from the start menu. You will then be asked for the Passphrase.