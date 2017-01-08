package processor;

import java.util.Queue;
import java.util.concurrent.Callable;
import java.util.concurrent.locks.Lock;

import domain.Node;

public class generateTaskHWJ1 implements Callable<Integer> {
	private Queue<Node> buffer;
	private OnerousProcessor processor;
	private Lock lock;
	private boolean bufferIsEmpty;

	public generateTaskHWJ1(Queue<Node> buffer, Lock lock, int max, boolean bufferIsEmpty) {
		this.buffer = buffer;
		this.lock = lock;
		this.processor = new FakeProcessor(max);
		this.bufferIsEmpty = bufferIsEmpty;
	}

	@Override
	public Integer call() throws Exception {
		// TODO Auto-generated method stub
		Integer result = 0;
		while(bufferIsEmpty == false) {
			Node current = null;
			lock.lock();
			try {
				if(buffer.isEmpty()) bufferIsEmpty = true;
				else {
					current = buffer.poll();
					if(current.getSx()!=null) {
						buffer.offer(current.getSx());
					}
					if(current.getDx()!=null) {
						buffer.offer(current.getDx());
					}
				}
			} finally {
				lock.unlock();
			}
			if(current != null) {
				result += this.processor.onerousFunction(current.getValue());
			}
		}
		return result;
	}

}
