/**
 * Created with IntelliJ IDEA.
 * Andrew Koe
 * Yodle Triangle puzzle
 */

import java.io.*;
import java.util.*;

public class Koe_Triangle {

    public static void main(String[] args) {
        String[] line;
        int[][] triangle = new int[100][100]; //memory inefficient and needs fixing later
        int i = 0;

        //parse through file
        try {
            File file = new File("/home/andrew/dev/triangle_puzzle/src/triangle.txt");
            Scanner scanner = new Scanner(file);
            while(scanner.hasNextLine()) {
                line = scanner.nextLine().split("\\s+");
                //Loop to take Strings from file and store them as ints in 2d array
                for(int j = 0; j < line.length;j++) {
                    triangle[i][j] = Integer.parseInt(line[j]);
                }
                i++;
            }
        }
        catch(FileNotFoundException e) {
            System.err.println("Could Not Open File");
            System.exit(-1);
        }

        //algorithm starts here
        /*
           Start from the base of the triangle and move up.  Do all of the sums but only let the greatest "bubble"
           up.  Leads to 9900 additions for a 100 row triangle.
         */
        for(int row = 98; row >= 0; row--) {
            for(int col = 0; col <= row; col++) {
                int child1 = triangle[row+1][col];
                int child2 = triangle[row +1][col+1];
                int root = triangle[row][col];
                triangle[row][col] = Math.max(child1 + root,child2 + root);
            }
        }
        System.out.println("The greatest sum is: " + triangle[0][0]);
    }
}
