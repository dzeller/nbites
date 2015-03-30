package nbtool.images;

import java.awt.image.BufferedImage;
import nbtool.images.YUYV8888image;

import java.util.Vector;
import java.awt.Graphics2D;
import java.awt.BasicStroke;
import java.awt.geom.Ellipse2D;


public final class Ballimage extends ImageParent {
    public int pixelSize() { return 2; }
    private Vector<String> balls = new Vector<String>();

    public Ballimage(int w, int h, byte[] d, Vector<String> ballLocs) {
	super(w, h, d);
	System.out.println("MAKING THE BALL IMAGE!!!!!!!!!!!");
	for(String s: ballLocs) {
	    // Strip leading and trailing braces
	    String toAdd = s.substring(1, s.length() - 1);
	    balls.add(toAdd);
	}
    }

    public BufferedImage toBufferedImage() {
	YUYV8888image yuv = new YUYV8888image(width, height, data);
	BufferedImage intermediate = yuv.toBufferedImage();

	Graphics2D g = intermediate.createGraphics();
	g.setStroke(new BasicStroke(5));

	for(String ball: balls) {
	    System.out.println("BALL IS: " + ball);
	    String[] split = ball.split(",");
	    if (split.length != 4) {
		System.out.println("Ballimage had malformed ball in it: " + ball);
		continue;
	    }
	    double centerX = Double.parseDouble(split[0]);
	    double centerY = Double.parseDouble(split[1]);
	    double radius = Double.parseDouble(split[2]);
	    double rating = Double.parseDouble(split[3]);

	    g.draw(new Ellipse2D.Double(radius, radius, centerX, centerY));
	}

	return intermediate;
    }

    public String encoding() {
	return "[Ball]";
    }
}