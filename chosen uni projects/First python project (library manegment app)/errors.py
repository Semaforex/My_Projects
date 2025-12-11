import global_variables as gl
from strings_and_messages import fail_message


class NegativeTimeLimitError(ValueError):
    def __init__(self):
        super().__init__(self, "Time Limit must be a positive integer.")

    def __str__(self):
        return "Time Limit must be a positive integer."


class BookReservedError(ValueError):
    def __init__(self):
        super().__init__(self, "You cannot postpone the return date of a book that is reserved.")

    def __str__(self):
        return "You cannot postpone the return date of a book that is reserved."


class NegativeExtentionsLeftError(ValueError):
    def __init__(self):
        super().__init__(self, "Extentions left cannot be negative.")

    def __str__(self):
        return "Extentions left cannot be negative."


class NegativeIdError(ValueError):
    def __init__(self):
        super().__init__(self, "Id cannot be negative.")

    def __str__(self):
        return "Id cannot be negative."


class EmptyTitleError(ValueError):
    def __init__(self):
        super().__init__(self, "Title cannot be empty.")

    def __str__(self):
        return "Title cannot be empty."


class EmptyAuthorError(ValueError):
    def __init__(self):
        super().__init__(self, "Author cannot be empty.")

    def __str__(self):
        return "Author cannot be empty."


class EmptyUsernameError(ValueError):
    def __init__(self):
        super().__init__(self, "Username cannot be empty.")

    def __str__(self):
        return "Username cannot be empty."


class TakenUsernameError(ValueError):
    def __init__(self):
        super().__init__(self, "Username already taken.")

    def __str__(self):
        return "Username already taken."


class EmptyPasswordError(ValueError):
    def __init__(self):
        super().__init__(self, "Password cannot be empty.")

    def __str__(self):
        return "Password cannot be empty."


class PasswordTooShortError(ValueError):
    def __init__(self):
        super().__init__(self, "Password must have at least 8 characters.")

    def __str__(self):
        return "Password must have at least 8 characters."


class BookNotAvailableError(ValueError):
    def __init__(self):
        super().__init__(self, "This book is not available.")

    def __str__(self):
        return "This book is not available."


class UserAlreadyInDatabaseError(ValueError):
    def __init__(self):
        super().__init__(self, "This user is already present in the database.")

    def __str__(self):
        return "This user is already present in the database."


class BookAlreadyInDatabaseError(ValueError):
    def __init__(self):
        super().__init__(self, "This book is already in the database.")

    def __str__(self):
        return "This book is already in the database."


class UserDoesNotExistError(ValueError):
    def __init__(self):
        super().__init__(self, "This user does not exist.")

    def __str__(self):
        return "This user does not exist."


class BookFileNotHere(FileNotFoundError):
    def __init__(self, filename):
        self._filename = filename
        super().__init__(self, f"There is no {filename} in './Put_Books_Here'")

    def __str__(self):
        return f"There is no {self._filename} in './Put_Books_Here'"


class BookNotInPossetionError(ValueError):
    def __init__(self):
        super().__init__(self, "You do not have this book in your possetion.")


class NoExtentionsLeftError(ValueError):
    def __init__(self):
        super().__init__(self, "You cannot extend the return date any more.")

    def __str__(self):
        return "You cannot extend the return date any more."


class WhitespaceError(ValueError):
    def __init__(self, type):
        super().__init__(self, f"{type} cannot contain spaces")
        self._type = type

    def __str__(self):
        return f"{self._type} cannot contain spaces"


class ForbittenUsernameError(ValueError):
    def __init__(self):
        super().__init__(self, "This username is forbitten")
        self._type = type

    def __str__(self):
        return "This username is forbitten"


def check_username(username: str):
    if not username:
        raise EmptyUsernameError
    if ' ' in username:
        raise WhitespaceError('username')
    if username == '!E':
        raise ForbittenUsernameError
    if username in [user.username for user in gl.database.user_list]:
        raise TakenUsernameError


def check_if_user_exists(username: str):
    if not username:
        raise EmptyUsernameError
    if username not in [user.username for user in gl.database.user_list]:
        raise UserDoesNotExistError


def check_password(password: str):
    if not password:
        raise EmptyPasswordError
    if ' ' in password:
        raise WhitespaceError('password')
    if len(password) < 8:
        raise PasswordTooShortError


def check_command_length(min: int, max: int, words: list[str]) -> bool:
    if '' in words:
        print(fail_message[4])
        return False
    if len(words) > max:
        print(fail_message[2])
        return False
    if len(words) < min:
        print(fail_message[3])
        return False
    return True
