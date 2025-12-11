from matplotlib import pyplot as plt
from matplotlib import ticker
import global_variables as gl


def plot_activity(ignore=None):
    """shows a bar chart of user activity which includes:
            -Borrowing a book
            -Returning a book
            -extending return date of a book
            -reserving a book """
    if not gl.database.available_books:
        print('No users in the database')
        return
    people = []
    values = []
    for customer in gl.database.customer_list:
        people.append(customer.username)
        values.append(customer.activity)
    fig, ax = plt.subplots()
    ax.yaxis.set_major_locator(ticker.MaxNLocator(integer=True))
    plt.bar(people, values, color='blue')
    plt.xlabel('customers')
    plt.ylabel('activity')
    plt.title('Activity')
    plt.show()


def plot_book_popularity(ignore=None):
    """shows a pie chart of all the books in the library (if the books
    have the same title and author they are treated as the same book).
    The ammount of pie taken by a book corresponds to it's popularity"""
    total_borrowed = 0
    dic = {}
    for book in gl.database.available_books:
        key = (f'{book.title} by {book.author}')
        dic[key] = dic.get(key, 0)+book.popularity
        total_borrowed += book.popularity
    if total_borrowed == 0:
        print('No books were borrowed')
        return
    labels = []
    sizes = []
    for key in dic:
        labels.append(key)
        sizes.append(dic[key]/total_borrowed)

    plt.pie(sizes, labels=labels, autopct='%1.1f%%', startangle=90)
    plt.axis('equal')
    plt.title("Book popularity")
    plt.show()
