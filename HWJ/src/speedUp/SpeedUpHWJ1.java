package speedUp;

import domain.BinaryTreeGenerator;
import domain.CountBinaryTree;
import domain.Node;
import processor.BinaryTreeAdder;
import processor.BinaryTreeAdderHWJ1;

public class SpeedUpHWJ1 {

	public SpeedUpHWJ1() {
		// TODO Auto-generated constructor stub
	}

	public static void speedUpTest(int lower, int higher, int lowerNB, int higherNB) {
		int numCPU = Runtime.getRuntime().availableProcessors();
		Node root;
		double speedUp;
		int maxDepth = 24;
		CountBinaryTree count = new CountBinaryTree();
		SpeedWatch speedWatch = new SpeedWatch();
		int i;
		BinaryTreeAdder treeSerial = new BinaryTreeAdderHWJ1(1, 2000); //seriale (con 1 cpu)
		BinaryTreeAdder treeConcurrent = new BinaryTreeAdderHWJ1(numCPU, 2000); //concorrente
		System.out.println("SPEED UP DI HWJ1");
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
		System.out.println("Albero binario fortemente sbilanciato");
		root = selectTree(false, 0, 1000); //albero binario fortemente sbilanciato, tutti i figli a destra.
		speedUp = speedWatch.getTime(root, treeSerial, treeConcurrent);
		System.out.println("nodi= "+count.countBinaryTreeNotBalanced(0, 1000)+", speed-up = "+speedUp);
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