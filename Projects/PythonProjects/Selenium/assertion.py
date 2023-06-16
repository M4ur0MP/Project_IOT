import unittest
from pyunitreport import HTMLTestRunner
from selenium import webdriver
from selenium.common.exceptions import NoSuchElementException
from selenium.webdriver.common.by import By


class AssertionsTest (unittest.TestCase):

    def setUp(self):
        self.driver = webdriver.Chrome(
            executable_path=r'C:/Users/mauro/Downloads/chromedriver_win32/chromedriver.exe')
        driver = self.driver
        driver.get("http://demo-store.seleniumacademy.com/")
        driver.maximize_window()

    def test_search_text_field(self):
        search_field = self.driver.find_element_by_id("search")

    def test_search_field(self):
        self.assertTrue(self.is_elemnts_present(By.NAME, 'q'))

    def test_lenguaje_option(self):
        self.assertTrue(self.is_elemnts_present(By.ID, 'select-leanguaje'))

    def tearDown(self):
        self.driver.quit()

    def is_elemnts_present(self, how, what):
        try:
            self.driver.find_element(by = how, value = what)
        except NoSuchElementException as variable:
            return False
        return True

if __name__ == "__main__":
    unittest.main(verbosity=2)
