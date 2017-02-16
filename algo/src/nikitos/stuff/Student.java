package nikitos.stuff;

public class Student implements Comparable<Student> {

    private String name, surname;
    private int grade, studentCard;
    private Gender gender;
    private boolean isMarried;

    public Student(String name, String surname, int grade, int studentCard, Gender gender, boolean isMarried){
        this.name = name;
        this.surname = surname;
        this.grade = grade;
        this.studentCard = studentCard;
        this.gender = gender;
        this.isMarried = isMarried;
    }

    @Override
    public int compareTo(Student st) {
        return studentCard - st.studentCard;
    }

    public int getStudentCard(){
        return studentCard;
    }

    @Override
    public String toString() {
        return name + " " + surname + ", gander: " + gender + ", grade: "
                + grade + ", student card id: " + studentCard + ", status: " + (isMarried ? "married" : "not married");
    }

    public int getGrade(){
        return grade;
    }

    public Gender getGender(){
        return gender;
    }

    public boolean isMarried(){
        return isMarried;
    }
}
