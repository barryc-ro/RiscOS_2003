import java.awt.*;
import java.applet.*;

public class TestIR extends Applet
{
  public boolean keyDown(Event evt, int key)
  {
    Graphics g = this.getGraphics();
    int width = size().width;
    int height = size().height;

    g.setColor(Color.white);
    g.fillRect(0, 0, width, height);
    g.setColor(Color.black);
    g.setFont(new Font("TimesRoman", Font.PLAIN, 16));

    String s = String.valueOf(key);
    FontMetrics currentFM = g.getFontMetrics();
    width = currentFM.stringWidth(s);
    g.drawString(s, width, height/2);
    return true;
  }
}
