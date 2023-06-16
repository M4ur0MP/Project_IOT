from typing import KeysView


def run():
    my_list = [1, "Hello", True, 4.5]
    my_dict = {"firstname": "Mauro", "lastname": "Martinez"}

    super_list = [
        {"firstname": "Facundo", "lastname": "Garcia"},
        {"firstname": "Gilberto", "lastname": "Rodriguez"},
        {"firstname": "Rhonda", "lastname": "Gonzalez"},
        {"firstname": "Tito", "lastname": "Chacaritas"},
        {"firstname": "El bocha", "lastname": "Ramones"}
    ]

    super_dict = {
        "natural_nums": [1, 2, 3, 4, 5],
        "integer_nums": [-1, 5, 0, 8, -9],
        "floating_nums": [1.1, 4.5, 6.3, 8.4]
    }

    for key, values in super_dict.items():
        print(key, "-", values)

    for k in super_list:
        print(k.get("firstname"), k.get("lastname"))


if __name__ == '__main__':
    run()
