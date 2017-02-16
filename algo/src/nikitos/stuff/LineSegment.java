package nikitos.stuff;

public class LineSegment {
    private double beginX, beginY, endX, endY;

    public LineSegment(double beginX, double beginY, double endX, double endY) {
        this.beginX = beginX;
        this.beginY = beginY;
        this.endX = endX;
        this.endY = endY;
    }

    public double length() {
        return Math.sqrt(Math.pow(beginX - endX, 2) + Math.pow(beginY - endY, 2));
    }

    public double angleWithOX() {
        return Math.toDegrees(Math.abs(Math.atan((endY - beginY) / (endX - beginX))));
    }

    @Override
    public String toString() {
        return "begin: x = " + beginX + ", y = " + beginY + "; end: x = " + endX + ", y = "
                + endY + "; length: " + length() + "; angle with OX: " + angleWithOX();
    }
}
