package org.example;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;
import java.util.Stack;
import java.util.HashSet;
import java.util.Set;

public class Main {
    final static ArrayList<Character> input = new ArrayList<>();
    final static Stack<Integer> brackets = new Stack<>();
    final static Set<Character> keywords = new HashSet<>(List.of('+', '-', '>', '<', ',', '.', '[', ']'));
    static final Scanner scanner = new Scanner(System.in);
    private static final int TAPE_SIZE = 30000;
    final static byte[] tape = new byte[TAPE_SIZE];
    static int inputIndex = 0;
    static int tapeIndex = 0;

    public static void main(String[] args) throws IOException {
        final ClassLoader classloader = Thread.currentThread().getContextClassLoader();
        final BufferedReader in = new BufferedReader(
                new InputStreamReader(classloader.getResource("mandelbrot.bf").openStream()));

        String inputLine;
        while ((inputLine = in.readLine()) != null)
            input.addAll(inputLine.chars().filter(c -> keywords.contains((char) c)).mapToObj(e -> (char) e).toList());

        interpret();

        scanner.close();
        in.close();
    }

    private static void interpret() {
        while (inputIndex < input.size()) {
            switch (input.get(inputIndex)) {
                case '+':
                    tape[tapeIndex]++;
                    break;
                case '-':
                    tape[tapeIndex]--;
                    break;
                case '>':
                    tapeIndex++;
                    break;
                case '<':
                    tapeIndex--;
                    break;
                case '.':
                    System.out.print((char) tape[tapeIndex]);
                    break;
                case ',':
                    tape[tapeIndex] = scanner.nextByte();
                    break;
                case '[':
                    // jump to closing ']'
                    if (tape[tapeIndex] == 0) {
                        int count = 1;
                        while (count != 0) {
                            inputIndex++;

                            if (input.get(inputIndex) == ']')
                                count--;
                            else if (input.get(inputIndex) == '[')
                                count++;
                        }
                    } else {
                        brackets.push(inputIndex);
                    }

                    break;
                case ']':
                    // jump to opening '['
                    if (tape[tapeIndex] != 0) {
                        inputIndex = brackets.peek();
                    } else {
                        brackets.pop();
                    }
                    break;
            }
            inputIndex++;
        }
    }
}