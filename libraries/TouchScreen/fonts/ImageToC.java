import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;

import javax.imageio.ImageIO;


public class ImageToC {

	private static final int HORIZONTAL = 0;
	private static final int VERTICAL = 1;
	
	private static final int BLACK_LIMIT = 128;
	
	private BufferedImage img = null;
	
	private String filename;
	private int bitsPerPixel;
	private int valuesPerLine;
	private int scanLineOrientation;
	private int fontWidth;
	private int fontCharSpacing;
	private int fontLineSpacing;
	private int firstChar;
	
	public ImageToC(String filename, int bitsPerPixel, int scanLineOrientation, int valuesPerLine, 
			int fontWidth, int fontCharSpacing, int fontLineSpacing, int firstChar) {
		this.filename = filename;
		this.bitsPerPixel = bitsPerPixel;
		this.scanLineOrientation = scanLineOrientation;
		this.valuesPerLine = valuesPerLine;
		this.fontWidth = fontWidth;
		this.fontCharSpacing = fontCharSpacing;
		this.fontLineSpacing = fontLineSpacing;
		this.firstChar = firstChar;
		try {
		    img = ImageIO.read(new File(filename));
		} catch (IOException e) {
			img = null;
			System.out.println("Error: can not load file named '" + filename + "'");
		}
	}

	public void outputData() {
		if (img == null) return;
		try {
			int w = img.getWidth(null);
			int h = img.getHeight(null);
			int[] rgbs = new int[w*h];
			img.getRGB(0, 0, w, h, rgbs, 0, w);
			PrintWriter out = getPrintWriter();
			int nbValues = 0;
			if (fontWidth != -1) outputFontData(out, rgbs, w, h);
			else {
				// we output the orientation legend
				outputOrientation(out);
				out.println();
				// we output the image
				out.println("#define IMAGE_WIDTH \t"+ w);
				out.println("#define IMAGE_HEIGHT\t"+ h);
				out.println("#define IMAGE_ORIENTATION\t"+ scanLineOrientation);
				out.println();
				if (bitsPerPixel == 16) {
					out.println("const unsigned short image[] = {");
					out.print("\t");
					if (scanLineOrientation == VERTICAL) {
						for (int x=0; x<w; x++) {
							for (int y=0; y<h; y++) {
								out.print(getRGB565(rgbs[y*w+x]) + ", ");
								nbValues ++;
								if (nbValues % valuesPerLine == 0) out.print("\n\t");
							}
						}
					} else { // HORIZONTAL or PAGE (which has no sense for 16 bits per pixel)
						for (int y=0; y<h; y++) {
							for (int x=0; x<w; x++) {
								out.print(getRGB565(rgbs[y*w+x]) + ", ");
								nbValues ++;
								if (nbValues % valuesPerLine == 0) out.print("\n\t");
							}
						}						
					}
				} else { // 1 bit per pixel
					out.println("const unsigned char image[] = {");
					out.print("\t");
					if (scanLineOrientation == PAGE) {
						int nbPages = h/8;
						if (h % 8 != 0) nbPages ++;
						for (int p=0; p<nbPages; p++) {
							for (int x=0; x<w; x++) {
								int value = 0;
								for (int y=0; y<8; y++) {
									if (p*8+y < h && getGrey(rgbs[(p*8+y)*w+x]) < BLACK_LIMIT) value += 1 << y;  
								}
								String hex = Integer.toHexString(value);
								if (hex.length() < 2) hex = "0" + hex;
								out.print("0x" + hex + ", ");
								nbValues ++;
								if (nbValues % valuesPerLine == 0) out.print("\n\t");
							}
						}
					} else if (scanLineOrientation == HORIZONTAL) {
						int value = 0;
						int bit = 0;
						for (int y=0; y<h; y++) {
							for (int x=0; x<w; x++) {
								if (getGrey(rgbs[y*w+x]) < BLACK_LIMIT) value += (1 << bit);
								bit ++;
								if (bit == 8 || x == w-1) {
									String hex = Integer.toHexString(value);
									if (hex.length() < 2) hex = "0" + hex;
									out.print("0x" + hex + ", ");
									nbValues ++;
									if (nbValues % valuesPerLine == 0) out.print("\n\t");
									value = 0;
									bit = 0;
								}
							}
						}
					} else { // VERTICAL
						int value = 0;
						int bit = 0;
						for (int x=0; x<w; x++) {
							for (int y=0; y<h; y++) {
								if (getGrey(rgbs[y*w+x]) < BLACK_LIMIT) value += (1 << bit);
								bit ++;
								if (bit == 8 || y == h-1) {
									String hex = Integer.toHexString(value);
									if (hex.length() < 2) hex = "0" + hex;
									out.print("0x" + hex + ", ");
									nbValues ++;
									if (nbValues % valuesPerLine == 0) out.print("\n\t");
									value = 0;
									bit = 0;
								}
							}
						}
						
					}
				}
			}
			out.println("};");
			out.flush();
			out.close();
			System.out.println(filename.substring(0, filename.lastIndexOf('.')) + ".h created with success");
		} catch (Exception e) {
			System.out.println("Error: " + e.getMessage());
		}		
	}
	
	private void outputOrientation(PrintWriter out) {
		out.println("/** Display fill direction");
		out.println(" *");			
		out.println(" *  0 (HORIZONTAL)");
		out.println(" *     --------/");
		out.println(" *     --------/");			
		out.println(" *");			
		out.println(" *  1 (VERTICAL)");
		out.println(" *     | /|");
		out.println(" *     |/ |");
		out.println(" *");			
		out.println(" *  2 (PAGE)");
		out.println(" *     |/|/|");
		out.println(" *     |/|/|");
		out.println(" **/");
		out.println();
		out.println("#define HORIZONTAL 0");
		out.println("#define VERTICAL 1");
		out.println("#define PAGE 2");

	}
	
	private void outputFontData(PrintWriter out, int rgbs[], int w, int h) {
		String fontName = filename.substring(0, filename.lastIndexOf('.'));
		if (fontName.indexOf('/') != -1) fontName = fontName.substring(fontName.lastIndexOf('/') + 1);
		else if (fontName.indexOf('\\') != -1) fontName = fontName.substring(fontName.lastIndexOf('\\') + 1);
		// font parameters
		int nbVerticalBytes = h/8;
		if (h % 8 != 0) nbVerticalBytes ++;
		int nbHorizontalBytes = fontWidth/8;
		if (fontWidth % 8 != 0) nbHorizontalBytes ++;
		int charWidth = fontWidth + fontCharSpacing;
		int nbChars = w / charWidth;
		// show orientation legend
		outputOrientation(out);
		out.println();
		// we output the font
		out.println("typedef struct {");
		out.println("\tuint8_t width;");
		out.println("\tuint8_t height;");
		out.println("\tuint8_t charSpacing;");
		out.println("\tuint8_t lineSpacing;");
		out.println("\tuint8_t firstChar;");
		out.println("\tuint8_t lastChar;");
		out.println("\tuint8_t orientation;");
		out.println("} fontDefinition_t;");
		out.println();
		out.print("const fontDefinition_t fontDefinition_" + fontName + " = { ");
		out.print(fontWidth + ", ");
		out.print(h + ", ");
		out.print(fontCharSpacing + ", ");
		out.print(fontLineSpacing + ", ");
		out.print("0x" + Integer.toHexString(firstChar) + ", ");
		out.print("0x" + Integer.toHexString(firstChar + nbChars - 1) + ", ");
		out.print(scanLineOrientation);
		out.println("};");
		out.println();
		out.print("const unsigned char font_" + fontName + "[][");
		if (scanLineOrientation == HORIZONTAL) out.print(h * nbHorizontalBytes);
		else out.print(nbVerticalBytes * fontWidth);
		out.println("] = {");
		for (int c=0; c<nbChars; c++) {
			out.print("\t{ ");
			boolean first = true;
			if (scanLineOrientation == PAGE) {
				for (int p=0; p<nbVerticalBytes; p++) {
					for (int x=0; x<fontWidth; x++) {
						int value = 0;
						for (int y=0; y<8; y++) {
							if (p*8+y < h && getGrey(rgbs[(p*8+y)*w+x+c*charWidth]) < BLACK_LIMIT) value += 1 << y;  
						}
						String hex = Integer.toHexString(value);
						if (hex.length() < 2) hex = "0" + hex;
						out.print((first ? "" : ", ") + "0x" + hex);
						first = false;
					}
				}
			} else if (scanLineOrientation == HORIZONTAL) {
				int value = 0;
				int bit = 0;
				for (int y=0; y<h; y++) {
					for (int x=0; x<fontWidth; x++) {
						if (getGrey(rgbs[y*w+x+c*charWidth]) < BLACK_LIMIT) value += (1 << bit);
						bit ++;
						if (bit == 8 || x == fontWidth-1) {
							String hex = Integer.toHexString(value);
							if (hex.length() < 2) hex = "0" + hex;
							out.print((first ? "" : ", ") + "0x" + hex);
							first = false;
							value = 0;
							bit = 0;
						}
					}
				}
			} else { // VERTICAL
				int value = 0;
				int bit = 0;
				for (int x=0; x<fontWidth; x++) {
					for (int y=0; y<h; y++) {
						if (getGrey(rgbs[y*w+x+c*charWidth]) < BLACK_LIMIT) value += (1 << bit);
						bit ++;
						if (bit == 8 || y == h-1) {
							String hex = Integer.toHexString(value);
							if (hex.length() < 2) hex = "0" + hex;
							out.print((first ? "" : ", ") + "0x" + hex);
							first = false;
							value = 0;
							bit = 0;
						}
					}
				}
			}
			out.println(" },\t\t/* " + ((char)(c + firstChar)) + 
					" - 0x" + Integer.toHexString(c + firstChar) + 
					" - " + (c + firstChar) + " */");
		}
	}
	
	private int getGrey(int rgb) {
		return (int) Math.round(0.21*((rgb & 0x00ff0000) >> 16) +
				0.71*((rgb & 0x0000ff00) >> 8) + 
				0.07*(rgb & 0x000000ff));		
	}
	
	private String getRGB565(int color) {
		int red5 = (color & 0x00ff0000) >> (16+3);
		int green6 = (color & 0x0000ff00) >> (8+2);
		int blue5 = (color & 0x000000ff) >> 3;
		int rgb565 = (red5 << 11) + (green6 << 5) + blue5;
		String hex = Integer.toHexString(rgb565);
		while (hex.length() < 4) hex = "0" + hex;
		return "0x" + hex;
	}
	
	private PrintWriter getPrintWriter() 
	throws IOException {
		String cFilename = filename.substring(0, filename.lastIndexOf('.')) + ".h";
		return new PrintWriter(new BufferedWriter(new FileWriter(cFilename)));
	}

	/**
	 * @param args
	 * 	0	filename
	 *  1	bitsPerPixel: 1, 16
	 *  2	scanLineOrientation:
	 *  		V: the display uses vertical scan lines
	 *  		H: the display uses horizontal scan lines
	 *  		P: the display used page scan lines
	 * 	3	valuesPerLine: number of columns in the array
	 *  4	fontWidth: width of a character
	 *  5	firstChar: if fontWidth, what is the first character
	 */
	public static void main(String[] args) {
		if (args.length < 4) {
			System.out.println("Usage: java -jar ImageToC.jar ImageToC filename bitsPerPixel " +
					"valuesPerLine scanLineOrientation [fontWidth [firstChar]]\n" +
					"\tfilename: name of the file to dump in C\n" +
					"\tbitsPerPixel: 1 or 16\n" +
					"\tscanLineOrientation: V for vertical or H for horizontal [default to H]\n" +
					"\t\tif V, the display will use vertical scan lines\n" + 
					"\t\tif H, the display will use horizontal scan lines\n" +
					"\t\tif P, the display will use page scan lines\n" +
					"\tvaluesPerLine: number of values per line in the output\n" +
					"\tfontWidth: width of a character in pixels (fontHeight is the height of the image)\n" +
					"\tfontCharSpacing: space between two characters in pixels\n" +
					"\tfontLineSpacing: space between two lines of characters in pixels\n" +
					"\tfirstChar: number of is the first character [default to 32: space]");
			return;
		}
		String filename = args[0];
		int bitsPerPixel = Integer.parseInt(args[1]);
		int valuesPerLine = Integer.parseInt(args[2]);
		int scanLineOrientation = HORIZONTAL;
		if (args[3].toUpperCase().charAt(0) == 'V') scanLineOrientation = VERTICAL;
		else if (args[3].toUpperCase().charAt(0) == 'P') scanLineOrientation = PAGE;
		int fontWidth = -1;
		if (args.length > 4) fontWidth = Integer.parseInt(args[4]);
		int fontCharSpacing = 1;
		if (args.length > 5) fontCharSpacing = Integer.parseInt(args[5]);
		int fontLineSpacing = 1;
		if (args.length > 6) fontLineSpacing = Integer.parseInt(args[6]);
		int firstChar = ' ';
		if (args.length > 7) firstChar = Integer.parseInt(args[7]);
		ImageToC converter = new ImageToC(filename, bitsPerPixel, scanLineOrientation, valuesPerLine,
				fontWidth, fontCharSpacing, fontLineSpacing, firstChar);
		converter.outputData();
	}

}
