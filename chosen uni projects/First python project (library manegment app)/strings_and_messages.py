help = {
    'start': '''
Commands:
    -Type in 'L'/'login' to log into an existing account
    -Type in 'C'/'create' to create a new account
    -Type in 'E'/'exit' to create a new account
    -Type in 'clear' to clear the database
''',
    'customer_session': '''
Commands:
    -Type in 'borrow [author] [book name]' to borrow a book
    -Type in 'return [author] [book name]' to return a book
    -Type in 'myinfo' to show information about your account
    -Type in 'E'/'exit' to log out and exit to main menu
    -Type in 'extend [book name]' to extend the reaturn deadline (up to 3 times)
    -Type in 'search [name fragments]/[author fragments]' search for a book
     (type in 'search' by itself to show all available books)
    -Type in 'delacc' to delete your account
''',
    'librarian_session': '''
Commands:
    -Type in 'E'/'exit' to log out and exit to main menu
    -Type in 'search [name fragments]/[author fragments]' to search for a book
     (type in 'search' by itself to show all available books)
    -Type in 'delacc' to delete your account
    -put a file containing the body of a book in
        the folder named 'Put_Books_Here' and
        type in 'add [author] [title] [filename] [holding time limit(deafult 30 days)]'
        to add the book into the library
    -Type in 'info [username]' to show info about a chosen customer
    -Type in 'delete [username]' to delete a choosen customer account
    -Type in 'delete [book name] [book author] to delete a choosen book
    -Type in 'popularity' to show a pie chart of book's popularity
    -Type in 'activity' to show a bar chart of user activity
    '''
}
starting_message = '''Welcome to the Library. Please choose one of the following commands:
login | create new account | help | exit [L/C/H/E]
>'''
input_q = {
    1: 'Are you a librarian or a customer? [L/C]\n>',
    2: 'You must choose either a librarian or a customer\n>',
    3: 'Pick a username\n>',
    4: 'Pick a password\n>',
    5: 'Please type in your username\n>',
    6: 'Please type in your password\n>',
    7: 'What would you like to do?\n>',
    8: 'Are you sure you want to delete your account?\n>'
}
fail_message = {
    1: 'Incorrect Password',
    2: 'Too many arguments',
    3: 'Not enought arguments',
    4: "Don't put two spaces next to each other in a command",
    5: "Cannot delete another librarian's accountn",
    6: 'This book is not in the database',
    7: "Cannot delete a book currently owned by a customer",
    8: "You already have this book in your possetion",
    9: 'Book not available',
    10: "Incorect Passphrase, could not continue with creating librarian Account\n",
    11: "This book is available, you do not need to reserve it",
    12: "You cannot reserve a book that you currently have",
    13: "Book already reserved by a user",
    14: "This book is not in your possetion",
    15: 'Time limit must be a number',
    16: 'There are no books in the database that meet the given criteria'
}

credentials_message = {
     1: "In order to create a librarian account, type in the Passphrase:\n>",
     2: "To clear the database you need to type in the Passphrase\n>"
}


def session_welcome(username: str) -> str:
    s = f'Welcome to the Library {username}!\n'
    s += '(Type "help" for available options)'
    return s


def format_string(s: str) -> str:
    """used for formating book title and author"""
    words = list(filter(('').__ne__, s.split('_')))
    return ' '.join([word.title() for word in words])
