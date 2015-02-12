package behavior_sim;

// A very basic SimObject

import java.awt.Color;

public class Ball extends SimObject
{
    public Ball() { super(); }
    public Ball(float xCoord, float yCoord) { super(xCoord, yCoord); }
    public Ball(Location location) { super(location); }

    protected void setRadiusAndColor()
    {
        color = Color.red;
        radius = 2.5f;
    }
}