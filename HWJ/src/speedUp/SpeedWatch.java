package speedUp;

import domain.Node;
import processor.BinaryTreeAdder;

public class SpeedWatch {

	public SpeedWatch() {
		// TODO Auto-generated constructor stub
	}
	
	public double getTime(Node root, BinaryTreeAdder serial, BinaryTreeAdder concurrent) {
		int i; 
		
		//serial
		for(i=0; i<5; i++) {
			serial.computeOnerousSum(root);
		}
		double start, stop, serialExecution = 0, concurrentExecution = 0;
		for(i=0; i<5; i++) {
			start = System.currentTimeMillis();
			serial.computeOnerousSum(root);
			stop = System.currentTimeMillis();
			serialExecution += stop - start;
		}
		double serialResult = serialExecution/5;
		//concurrent
		for(i=0; i<5; i++) {
			concurrent.computeOnerousSum(root);
		}
		for(i=0; i<5; i++) {
			start = System.currentTimeMillis();
			concurrent.computeOnerousSum(root);
			stop = System.currentTimeMillis();
			concurrentExecution += stop-start;
		}
		double concurrentResult = concurrentExecution/5;
		//speed up
		double result = serialResult / concurrentResult;
		return result;
	}
}
