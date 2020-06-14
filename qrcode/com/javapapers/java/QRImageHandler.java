package com.javapapers.java;

import com.google.zxing.*;
import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.oned.UPCEReader;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;
import com.pqscan.barcodereader.BarCodeType;
import com.pqscan.barcodereader.BarcodeResult;
import com.pqscan.barcodereader.BarcodeScanner;

import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.lang.reflect.Array;
import java.nio.Buffer;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Stream;

public class QRImageHandler {
    private Rectangle posAndSize = new Rectangle();

    public void setQRPosAndSize(int x, int y, int width, int height) {
        this.posAndSize = new Rectangle(x,y,width,height);
    }

    public BufferedImage getImage(String filepath) throws IOException {
        BufferedImage image = ImageIO.read(new FileInputStream(filepath));
        image = cutImage(image);
        image = scaleImage(image, 200);

        return image;
    }

    public ArrayList<Byte> getQRBytes2(String filename) throws IOException {

        ArrayList<Byte> b = new ArrayList<>();

        BarcodeResult[] results = BarcodeScanner.Scan(getImage(filename));

        for (BarcodeResult result : results) {
            System.out.println(result.getData());
        }

        return b;
    }

    public ArrayList<Byte> getQRBytes(String filename) throws NotFoundException, IOException {
        ArrayList<Byte> QRByteArrdata = new ArrayList<>();

        for (byte b :  readQR(this.getImage(filename)).getRawBytes())
            QRByteArrdata.add(b);

        return QRByteArrdata;
    }

    public static Result readQR(BufferedImage image) throws NotFoundException {
        Hashtable<DecodeHintType, Object> decodeHints = new Hashtable<>();
        decodeHints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
        //decodeHints.put(DecodeHintType.POSSIBLE_FORMATS, EnumSet.allOf(BarcodeFormat.class));
        decodeHints.put(DecodeHintType.POSSIBLE_FORMATS, EnumSet.of(BarcodeFormat.AZTEC));

        BinaryBitmap binaryBitmap = new BinaryBitmap(
                new HybridBinarizer(
                        new BufferedImageLuminanceSource(image)));

        return new MultiFormatReader().decode(binaryBitmap, decodeHints);
    }

    private BufferedImage cutImage(BufferedImage image) {
        return image.getSubimage(posAndSize.x, posAndSize.y, posAndSize.width, posAndSize.height);
    }

    private BufferedImage scaleImage(BufferedImage image, int size) {
        return Scalr.resize(image, size);
    }

    private static void displayImage(BufferedImage image) {
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
