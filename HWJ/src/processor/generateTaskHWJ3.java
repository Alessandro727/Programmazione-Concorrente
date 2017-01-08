package processor;

import java.util.Deque;
import java.util.LinkedList;
import java.util.concurrent.RecursiveTask;

import domain.Node;

@SuppressWarnings("serial")
public class generateTaskHWJ3 extends RecursiveTask<Integer> {
	private Deque<Node> buffer;
	Node root;
	int max;
	OnerousProcessor processor;
	static final int SEQUENTIAL_THRESHOLD = 5;
	
	
	public generateTaskHWJ3(Node root, int max) {
		this.root = root;
		this.max = max;
		this.processor = new FakeProcessor(max);
		this.buffer = new LinkedList<Node>();
		this.buffer.offerFirst(root);
	}

	@Override
	protected Integer compute() {
		Node current = this.buffer.pollFirst();
		if (getDepthNode(current,0)<SEQUENTIAL_THRESHOLD) {
			int result = 0;
			while (true) {
				if (current==null)
					current = this.buffer.pollFirst();
				if (current==null) {
					break;
				}
				if (current.getSx()!=null) {
					this.buffer.offerLast(current.getSx());
				}
				if (current.getDx()!=null) {
					this.buffer.offerLast(current.getDx());
				}
				result+=processor.onerousFunction(current.getValue());
				current=null;
			}
			return result;
		}
		else {
			int resDx = 0;
			int resSx = 0;
			generateTaskHWJ3 left = null;
			if (current.getSx()!=null) {
				left = new generateTaskHWJ3(current.getSx(), max);
				left.fork();
			}
			if (current.getDx()!=null) {
				generateTaskHWJ3 right = new generateTaskHWJ3(current.getDx(), max);
				resDx = right.compute();
			}
			if (left!=null)
				resSx = left.join();
			return processor.onerousFunction(current.getValue()) + resSx + resDx;
		}
	}
	
	/* test soglia */
	private static int getDepthNode(Node n, int depth) {
		if(n==null)
			return depth-1;
		
		if(depth==SEQUENTIAL_THRESHOLD)
			return depth;
		
		return Math.max(getDepthNode(n.getSx(),depth+1), getDepthNode(n.getDx(),depth+1));
	}
	
}
