from user import Librarian, Customer, User
from book import Book
import copy
from database import Database
from plot import plot_activity, plot_book_popularity
import global_variables as gl
from strings_and_messages import (
    help,
    input_q,
    starting_message,
    credentials_message,
    session_welcome,
    fail_message,
)
from errors import (
    check_password,
    check_username,
    check_if_user_exists,
    check_command_length,
    UserDoesNotExistError,
)


def pick_password_interface() -> str:
    while True:
        password = input(input_q[4])
        try:
            check_password(password)
            return password
        except Exception as e:
            print(str(e))


def pick_name_interface() -> str:
    while True:
        username = input(input_q[3])
        try:
            check_username(username)
            return username
        except Exception as e:
            print(str(e))


def verify_credentials(passphrase: str) -> bool:
    if passphrase == gl.PASSPHRASE:
        return True
    return False


def create_account_interface() -> User:
    mode = input(input_q[1]).title()
    while mode not in ['L', 'C', 'Librarian', 'Customer']:
        mode = input(input_q[2]).title()
    match mode:
        case 'L' | 'Librarian':
            passphrase = input(credentials_message[1])
            if verify_credentials(passphrase):
                new_user = Librarian(pick_name_interface(), pick_password_interface())
                gl.database.add_librarian(new_user)
            else:
                print(fail_message[10])
                start()
        case 'C' | 'Customer':
            new_user = Customer(pick_name_interface(), pick_password_interface())
            gl.database.add_customer(new_user)
    if type(new_user) is Customer:
        customer_interface(new_user)
    else:
        librarian_interface(new_user)


def login_username_interface() -> str:
    username = None
    while True:
        try:
            username = input(input_q[5])
            if username == '!E':
                return username
            check_if_user_exists(username)
            return gl.database.user_with_username(username)
        except Exception as e:
            print(str(e))


def login_interface():
    user = login_username_interface()
    if user == '!E':
        return
    succes = False
    while not succes:
        password = input(input_q[6])
        if password == '!E':
            return
        if user.check_password_correct(password):
            succes = True
        else:
            print(fail_message[1])
    if type(user) is Customer:
        customer_interface(user)
    else:
        librarian_interface(user)


def delete_interface(command: (User, str)):
    words = command[1].split(' ')
    words.remove(words[0])
    if not check_command_length(1, 2, words):
        return
    try:
        print(gl.database.delete(words))
    except Exception as e:
        print(str(e))


def user_info_interface(command: (User, str)):
    words = command[1].split(' ')
    if not check_command_length(2, 2, words):
        return
    try:
        user = gl.database.user_with_username(words[1])
        print(repr(user))
    except UserDoesNotExistError as e:
        print(str(e))


def borrow_interface(command: (User, str)):
    user = command[0]
    words = command[1].split(' ')
    if not check_command_length(3, 3, words):
        return
    if user.check_if_in_possetion(words[1], words[2]):
        print(fail_message[8])
        return
    book = gl.database.check_if_available(words[1], words[2])
    if not book:
        print(fail_message[9])
        return
    user.borrow_book(book)
    print(f'Successfully borrowed {book.title} by {book.author}\n')


def return_interface(command: (User, str)):
    user = command[0]
    words = command[1].split(' ')
    if not check_command_length(3, 3, words):
        return
    book = user.check_if_in_possetion(words[1], words[2])
    if not book:
        print(fail_message[14])
        return
    user.return_book(book)
    print('Success\n')


def reserve_interface(command: (User, str)):
    words = command[1].split(' ')
    user = command[0]
    if not check_command_length(3, 3, words):
        return
    books = gl.database.check_if_book_in_database(words[1], words[2])
    if books:
        for book in books:
            if book in user.books:
                print(fail_message[12])
                return
    else:
        print(fail_message[6])
        return
    book = gl.database.check_if_available(words[1], words[2])
    if book:
        print(fail_message[11])
        return
    success = user.reserve_book(books[0])
    if not success:
        print(fail_message[13])
    else:
        print("Book reserved successfully\n")


def extend_interface(command: (User, str)):
    words = command[1].split(' ')
    user = command[0]
    if not check_command_length(3, 3, words):
        return
    book = user.check_if_in_possetion(words[1], words[2])
    if not book:
        print(fail_message[14])
        return
    try:
        user.extend_return_date(book)
        print("Success!\n")
    except Exception as e:
        print(str(e))


def search_interface(command: (User, str)):
    words = command[1].split(' ')
    if len(words) == 1:
        if not gl.database.available_books:
            print(fail_message[16])
            return
        for book in gl.database.available_books:
            print(repr(book))
        return
    to_print = []
    for word in words[1::]:
        for book in gl.database.available_books:
            if word.lower() in f'{book.title} {book.author}'.lower() and not word == '':
                if book not in to_print:
                    to_print.append(book)
    if not to_print:
        print(fail_message[16])
        return
    for book in to_print:
        print(repr(book))


def delete_account_interface(command: (User, str)):
    user = command[0]
    if user.check_password_correct(input(input_q[6])):
        if input(input_q[8]).title() in ['Y', 'Yes']:
            if type(user) is Customer:
                books_copy = copy.copy(user.books)
                for book in books_copy:
                    user.return_book(book)
                for book in gl.database.available_books:
                    if user.username == book.reserved:
                        book.set_reserved()
                        gl.database.update_book(book)
            gl.database.remove_account(user)
            logout()
        return
    print(fail_message[1])


def add_book_interface(command: (User, str)):
    words = command[1].split(' ')
    if not check_command_length(4, 5, words):
        return
    author = words[1]
    title = words[2]
    body_file = words[3]
    if len(words) == 4:
        try:
            gl.database.add_book(Book(author, title), body_file)
            print("Successfully added the book\n")
        except Exception as e:
            print(str(e))
    else:
        try:
            time_limit = int(words[4])
        except ValueError:
            print(fail_message[15])
            return
        try:
            gl.database.add_book(Book(author, title, time_limit), body_file)
            print("Successfully added the book\n")
        except Exception as e:
            print(str(e))


def clear_interface():
    passphrase = input(credentials_message[2])
    if verify_credentials(passphrase):
        gl.database.clear()
    else:
        print('wrong passphrase')


def print_start_help():
    print(help['start'])


def print_wrong_command(ignore=None):
    print('Command not found')


def print_customer_help(ingore=None):
    print(help['customer_session'])


def print_librarian_help(ignore=None):
    print(help['librarian_session'])


def logout(ignore=None):
    print('\n')
    raise StopIteration


def print_userinfo(comamnd: (User, str)):
    print(repr(comamnd[0]))


def customer_interface(user: Customer):
    command = None
    commands = {
        'H': print_customer_help, 'Help': print_customer_help,
        'Myinfo': print_userinfo,
        'Borrow': borrow_interface,
        'Return': return_interface,
        'Extend': extend_interface,
        'Search': search_interface,
        'Reserve': reserve_interface,
        'Delacc': delete_account_interface,
        'E': logout, 'Exit': logout
    }
    print(session_welcome(user.username))
    while True:
        command = input(input_q[7])
        word = command.split(' ')[0].title()
        try:
            commands.get(word, print_wrong_command)((user, command))
        except StopIteration:
            break


def librarian_interface(user: Librarian):
    commands = {
        'H': print_librarian_help, 'Help': print_librarian_help,
        'Add': add_book_interface,
        'Info': user_info_interface,
        'Search': search_interface,
        'Activity': plot_activity,
        'Popularity': plot_book_popularity,
        'Delete': delete_interface,
        'Delacc': delete_account_interface,
        'E': logout, 'Exit': logout
    }
    command = None
    print(session_welcome(user.username))
    while True:
        command = input(input_q[7])
        word = command.split(' ')[0].title()
        try:
            commands.get(word, print_wrong_command)((user, command))
        except StopIteration:
            break


def start():
    command = None
    commands = {
        'L': login_interface, 'Login': login_interface,
        'C': create_account_interface, 'Create': create_account_interface,
        'H': print_start_help, 'Help': print_start_help,
        'Clear': clear_interface,
        'E': exit, 'Exit': exit
    }
    while True:
        command = input(starting_message).title()
        commands.get(command, print_wrong_command)()


def main():
    gl.database = Database()
    gl.database.setup()
    start()


if __name__ == "__main__":
    main()
