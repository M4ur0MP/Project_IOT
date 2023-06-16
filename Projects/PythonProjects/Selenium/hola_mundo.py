import unittest
from pyunitreport import HTMLTestRunner
from selenium import webdriver


class HelloWorld (unittest.TestCase):

    @classmethod
    def setUpClass(cls) -> None:
        cls.driver = webdriver.Chrome(
            executable_path=r'C:/Users/mauro/Downloads/chromedriver_win32/chromedriver.exe')
        driver = cls.driver
        driver.implicitly_wait(10)

    def test_hello_world(self):
        driver = self.driver
        driver.get('http://dominio.ddns.net/quesos-web')

    def test_cocreando(self):
        self.driver.get('http://cocreandotuser.me')

    @classmethod
    def tearDownClass(cls):
        cls.driver.quit()


if __name__ == "__main__":
    unittest.main(verbosity=2, testRunner=HTMLTestRunner(
        output='reportes', report_name='hello_world'))
