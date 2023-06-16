package Java;

public class Account {
    Integer id;
    String name;
    String document;
    String email;
    String password;

    public Account(String name, String document) {
        this.name = name;
        this.document = document;
    }

    @Override
    public String toString() {
        return "Account [document=" + document + ", email=" + email + ", id=" + id + ", name=" + name + ", password="
                + password + "]";
    }

}
