package processor;

import java.util.concurrent.ForkJoinPool;

import domain.Node;

public class BinaryTreeAdderHWJ3 implements BinaryTreeAdder{
	final ForkJoinPool pool;
	int max;
	
	public BinaryTreeAdderHWJ3(int numCpu, int max) {
		this.max = max;
		pool = new ForkJoinPool(numCpu);
	}

	@Override
	public int computeOnerousSum(Node root) {
		if (root == null)
			return 0;
		return pool.invoke(new generateTaskHWJ3(root, max));
	}

}
