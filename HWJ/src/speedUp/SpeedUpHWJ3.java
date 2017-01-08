package speedUp;

import domain.Node;
import domain.BinaryTreeGenerator;
import domain.CountBinaryTree;
import processor.BinaryTreeAdder;
import processor.BinaryTreeAdderHWJ3;

public class SpeedUpHWJ3 {
	public SpeedUpHWJ3() {}
	
	public static void speedUpTest(int lower, int higher, int lowerNB, int higherNB) {
		int numCPU = Runtime.getRuntime().availableProcessors();
		Node root;
		double speedUp;
		int maxDepth = 24;
		CountBinaryTree count = new CountBinaryTree();
		SpeedWatch speedWatch = new SpeedWatch();
		int i;
		BinaryTreeAdder treeSerial = new BinaryTreeAdderHWJ3(1, 2000); //seriale (con 1 cpu)
		BinaryTreeAdder treeConcurrent = new BinaryTreeAdderHWJ3(numCPU, 2000); //concorrente
		System.out.println("SPEED UP DI HWJ3");
		System.out.println("Albero binario bilanciato");
		for(i = lower; i<=higher; i++) {
			root = selectTree(true, i, i);
			speedUp = speedWatch.getTime(root, treeSerial, treeConcurrent);
			System.out.println("nodi = "+ count.countBinaryTreeBalanced(i) + ", speed-up = "+speedUp);
		}
		System.out.println("Albero binario sbilanciato");
		for(i = lowerNB; i<=higherNB; i++) {
			root = selectTree(false, i, maxDepth); //albero binario non bilanciato
			speedUp = speedWatch.getTime(root, treeSerial, treeConcurrent);
			System.out.println("nodi= "+count.countBinaryTreeNotBalanced(i, maxDepth)+", speed-up = "+speedUp);
		}
	}
	
	public static Node selectTree(boolean isBalanced, int depthSx, int depthDx) {
		Node root;
		BinaryTreeGenerator tree = new BinaryTreeGenerator();
		if(isBalanced) 
			root = tree.createBalancedBinaryTree(depthSx);
		else 
			root = tree.createNotBalancedBinaryTree(depthSx, depthDx);
		return root;
	}
	
	public static void main(String[] args) {
		int lower = 10; 
		int higher = 15;
		int lowerNB = 7;
		int higherNB = 12;
		speedUpTest(lower, higher, lowerNB, higherNB);
	}
}
