package domain;

public class CountBinaryTree {

	public CountBinaryTree() {
		// TODO Auto-generated constructor stub
	}
	
	public int countBinaryTreeBalanced(int depth) {
		return (int)Math.pow(2, depth+1)-1;
	}
	
	public int countBinaryTreeNotBalanced(int depthSx, int depthDx) {
		int minDepth = Math.min(depthSx, depthDx);
		int maxDepth = Math.max(depthDx, depthSx);
		return ((int) Math.pow(2, minDepth+1))-1+((int) Math.pow(2, minDepth))*(maxDepth-minDepth);
	}
}
