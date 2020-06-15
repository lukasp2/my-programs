package com.javapapers.java;

import com.google.zxing.*;
import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
import com.google.zxing.common.HybridBinarizer;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.image.AffineTransformOp;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Stream;

public class QRImageHandler {
    private Rectangle posAndSize = new Rectangle();
    private int rotation = 0;

    public void setQRPosAndSize(int x, int y, int width, int height, int rotation) {
        this.posAndSize = new Rectangle(x,y,width,height);
        this.rotation = rotation;
    }

    public BufferedImage getImage(String filepath) throws IOException {
        BufferedImage image = ImageIO.read(new FileInputStream(filepath));
        image = cutImage(image);
        image = addPadding(image);
        image = scaleImage(image, 1000);

        for (int i = 0; i < rotation; ++i) { image = rotateImage90CW(image); }

        return image;
    }

    public ArrayList<Byte> getQRBytes(BufferedImage image) throws NotFoundException {
        Hashtable<DecodeHintType, Object> decodeHints = new Hashtable<>();
        decodeHints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
        decodeHints.put(DecodeHintType.POSSIBLE_FORMATS, EnumSet.of(BarcodeFormat.AZTEC));
        decodeHints.put(DecodeHintType.PURE_BARCODE, Boolean.FALSE);

        BinaryBitmap binaryBitmap = new BinaryBitmap(new HybridBinarizer(new BufferedImageLuminanceSource(image)));
        binaryBitmap.rotateCounterClockwise();
        binaryBitmap.rotateCounterClockwise();

        Result result = new MultiFormatReader().decode(binaryBitmap, decodeHints);

        ArrayList<Byte> QRByteArrdata = new ArrayList<>();
        for (byte b :  result.getRawBytes())
            QRByteArrdata.add(b);

        return QRByteArrdata;
    }

    private BufferedImage rotateImage90CW(BufferedImage image) {
        double rotation = Math.toRadians (90);
        double locationX = image.getWidth() / 2;
        double locationY = image.getHeight() / 2;

        AffineTransform tx = AffineTransform.getRotateInstance(rotation, locationX, locationY);
        AffineTransformOp op = new AffineTransformOp(tx, AffineTransformOp.TYPE_BILINEAR);

        return op.filter(image, null);
    }

    private BufferedImage cutImage(BufferedImage image) {
        return image.getSubimage(posAndSize.x, posAndSize.y, posAndSize.width, posAndSize.height);
    }

    private BufferedImage scaleImage(BufferedImage image, int size) {
        return Scalr.resize(image, size);
    }

    // returns a the input image with a 50px white padding
    public BufferedImage addPadding(BufferedImage image) {
        int padding = 80;

        BufferedImage paddingImage = new BufferedImage(image.getWidth() + padding * 2,image.getHeight() + padding * 2, BufferedImage.TYPE_INT_RGB);
        Graphics2D graphics = paddingImage.createGraphics();
        graphics.setPaint(new Color( 255, 255, 255 ));
        graphics.fillRect(0, 0, paddingImage.getWidth(), paddingImage.getHeight());
        graphics.drawImage(image,padding,padding,null);

        return paddingImage;
    }

    public void displayImage(BufferedImage image) {
        ImageIcon icon = new ImageIcon(image);
        JFrame frame = new JFrame();
        frame.setLayout(new FlowLayout());
        frame.setSize(image.getWidth(),image.getHeight() + 50);
        JLabel lbl = new JLabel();
        lbl.setIcon(icon);
        frame.add(lbl);
        frame.setVisible(true);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
    }

    public ArrayList<String> getFilesInDir(String directory) {
        ArrayList<String> fileNames = new ArrayList<>();

        try (Stream<Path> paths = Files.walk(Paths.get(directory))) {
            Object[] fileNamesObjArr = paths.filter(Files::isRegularFile).toArray();

            for (Object o : fileNamesObjArr) {
                fileNames.add(o.toString());
            }
        }
        catch (IOException ioException) {
            ioException.printStackTrace();
        }

        return fileNames;
    }
}
