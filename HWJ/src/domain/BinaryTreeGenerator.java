package domain;

public class BinaryTreeGenerator {

	public BinaryTreeGenerator() {
		// TODO Auto-generated constructor stub
	}

	public Node createBalancedBinaryTree(int depth) {
		if(depth<0) {
			return null;
		}
		else {
			return new NodeImpl(1, createBalancedBinaryTree(depth-1), createBalancedBinaryTree(depth-1));
		}
	}
	
	public Node createNotBalancedBinaryTree(int depthSx,int depthDx) {
		if (depthSx>=0)
			return new NodeImpl(1, createNotBalancedBinaryTree(depthSx-1, depthDx-1),
					createNotBalancedBinaryTree(depthDx-1, depthSx-1));
		else
			return null;
	}
}
