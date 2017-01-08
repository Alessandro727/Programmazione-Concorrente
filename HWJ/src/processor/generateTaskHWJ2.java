package processor;

import java.util.Deque;
import java.util.List;
import java.util.Map;
import java.util.concurrent.Callable;
import java.util.concurrent.locks.Lock;

import domain.Node;

public class generateTaskHWJ2 implements Callable<Integer> {
	private Map<Integer, Deque<Node>> buffers;
	private int threadNumber;
	private OnerousProcessor processor;
	private int cpu;
	private boolean bufferIsEmpty;
	private List<Lock> buffersLock;
	private Lock steelWorkLock;

	public generateTaskHWJ2(Map<Integer, Deque<Node>> buffers, int threadNumber, int max, int cpu, boolean bufferIsEmpty, List<Lock> buffersLock, Lock steelWorkLock) {
		// TODO Auto-generated constructor stub
		this.buffers = buffers;
		this.threadNumber = threadNumber;
		this.processor = new FakeProcessor(max);
		this.cpu = cpu;
		this.bufferIsEmpty = bufferIsEmpty;
		this.buffersLock = buffersLock;
		this.steelWorkLock = steelWorkLock;
	}

	@Override
	public Integer call() throws Exception {
		Deque<Node> myBuffer = this.buffers.get(threadNumber);
		int result = 0;
		while(bufferIsEmpty == false) {
			Node current = null;
			this.buffersLock.get(threadNumber).lock();
			try {
				current = myBuffer.pollLast();
				if(current!=null) {
					if(current.getDx() != null) {
						myBuffer.offerLast(current.getDx());
					}
					if(current.getSx()!=null) {
						myBuffer.offerLast(current.getSx());
					}
				}
			} finally {
				this.buffersLock.get(threadNumber).unlock();
			}
			if(current == null) {
				this.steelWorkLock.lock();
				try {
					current = stealsWork();
				} finally {
					this.steelWorkLock.unlock();
				}
			}
			if(current == null) {
				bufferIsEmpty = true;
			}
			if(current != null) {
				result += processor.onerousFunction(current.getValue());
			}
		}
		return result;
	}

	private Node stealsWork() {
		// TODO Auto-generated method stub
		Node stolen = null;
		this.buffersLock.get(threadNumber).lock();
		for(int i = 0; i<cpu; i++) {
			if(i != threadNumber) {
				this.buffersLock.get(i).lock();
				stolen = buffers.get(i).pollFirst();
				if(stolen!=null) {
					if(stolen.getDx() != null) {
						this.buffers.get(threadNumber).offerLast(stolen.getDx());
					}
					if(stolen.getSx() != null) {
						this.buffers.get(threadNumber).offerLast(stolen.getSx());
					}
				}
				this.buffersLock.get(i).unlock();
				if(stolen != null) 
					break;
			}
		}
		this.buffersLock.get(threadNumber).unlock();
		return stolen;
	}

}
