package com.javapapers.java;

import java.awt.image.BufferedImage;
import java.io.IOException;
import java.util.ArrayList;

import com.google.zxing.NotFoundException;

public class Main {
	public static void main(String[] args) throws IOException, NotFoundException {

		QRImageHandler qrImageHandler = new QRImageHandler();

		//works
		//ArrayList<String> filenames = qrImageHandler.getFilesInDir("QRscreenshots/nkpg-rim/");
		//qrImageHandler.setQRPosAndSize(0,0,200,200, 0);

		qrImageHandler.setQRPosAndSize(130,414,378,378, 0);
		ArrayList<String> filenames = qrImageHandler.getFilesInDir("QRscreenshots/rim-nkpg/");

		ArrayList<ArrayList<Byte>> QRBytes = new ArrayList<>();

		for (String filename : filenames) {
			System.out.println(filename);
			BufferedImage image = qrImageHandler.getImage(filename);
			qrImageHandler.printImageInfo(image);

			ArrayList<Byte> imageBytes = qrImageHandler.getQRBytes(image);
			if (!imageBytes.isEmpty()) { QRBytes.add(imageBytes); }
		}

		for (int i = 0; i < QRBytes.size(); ++i) {
			System.out.println("QR code " + i + " bytes: " + QRBytes.get(i));
			System.out.println("QR code " + i + " length: " + QRBytes.get(i).size() + "\n");
		}

		ArrayList<Byte> matchingBytes = getMatchingBytes(QRBytes.get(0), QRBytes.get(1));
		if (QRBytes.size() > 2) {
			for (int i = 2; i < QRBytes.size(); ++i) {
				matchingBytes = getMatchingBytes(matchingBytes, QRBytes.get(i));
			}
		}
		ArrayList<ArrayList<Byte>> matchingBytesInBlocks = getMatchingBytesInBlocks(matchingBytes, QRBytes.get(QRBytes.size() - 1));

		System.out.println(matchingBytesInBlocks.size() + " matches found:");
		for (int i = 0; i < matchingBytesInBlocks.size(); ++i) {
			System.out.println(matchingBytesInBlocks.get(i));
		}
	}

	// for two arrays a1 = [1,2,3,-1,-1,4,5,6,-1,7,8,9]
	//                a2 = [1,2,3,-2,-2,4,5,6,-2,-2,-2,7,8,9]
	// this will return    [1,2,3,4,5,6,7,8,9]
	public static ArrayList<Byte> getMatchingBytes(ArrayList<Byte> arr1, ArrayList<Byte> arr2) {
		ArrayList<Byte> allMatchingBytes = new ArrayList<>();

		for (int i = 0; i < arr1.size(); ++i) {
			for (int k = 0; k < arr2.size(); ++k) {

				ArrayList<Byte> matchingBytes = new ArrayList<>();

				while (arr1.get(i).equals(arr2.get(k))) {
					matchingBytes.add(arr1.get(i));

					if (++i >= arr1.size() || ++k >= arr2.size())
						break;
				}

				if (matchingBytes.size() >= 3) {
					allMatchingBytes.addAll(matchingBytes);
				}

				if (i >= arr1.size() || k >= arr2.size())
					break;
			}
		}

		return allMatchingBytes;
	}

	// for two arrays a1 = [1,2,3,-1,-1,4,5,6,-1,7,8,9]
	//                a2 = [1,2,3,-2,-2,4,5,6,-2,-2,-2,7,8,9]
	// this will return    [[1,2,3][4,5,6][7,8,9]]
	public static ArrayList<ArrayList<Byte>> getMatchingBytesInBlocks(ArrayList<Byte> arr1, ArrayList<Byte> arr2) {
		int MIN_BLOCK_SIZE = 3;

		ArrayList<ArrayList<Byte>> allMatchingBytes = new ArrayList<>();

		for (int i = 0; i < arr1.size(); ++i) {
			for (int k = 0; k < arr2.size(); ++k) {

				ArrayList<Byte> matchingBytes = new ArrayList<>();

				while (arr1.get(i).equals(arr2.get(k))) {
					matchingBytes.add(arr1.get(i));

					if (++i >= arr1.size() || ++k >= arr2.size())
						break;
				}

				if (matchingBytes.size() >= MIN_BLOCK_SIZE) {
					allMatchingBytes.add(matchingBytes);
				}

				if (i >= arr1.size() || k >= arr2.size())
					break;
			}
		}

		return allMatchingBytes;
	}
}
