package processor;

import domain.Node;

import java.util.LinkedList;
import java.util.Queue;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BinaryTreeAdderHWJ1 implements BinaryTreeAdder {
	private Queue<Node> buffer;
	int cpu;
	int max; //max di FakeProcessor

	public BinaryTreeAdderHWJ1(int cpu, int max) {
		// TODO Auto-generated constructor stub
		this.cpu = cpu;
		this.max = max; 
		this.buffer = new LinkedList<Node>();
	}

	@Override
	public int computeOnerousSum(Node root) {
		// TODO Auto-generated method stub
		boolean bufferIsEmpty = false;
		if(root == null) {
			return 0;
		}
		int result = 0;
		Lock lock = new ReentrantLock(); 
		ExecutorService executor = Executors.newFixedThreadPool(cpu);
		// con ecs gestisco i task generati processando in ordine di completamento e non di creazione
		CompletionService<Integer> ecs = new ExecutorCompletionService<Integer>(executor); 
		this.buffer.offer(root);
		for(int i = 0; i < cpu; i++) {
			ecs.submit(new generateTaskHWJ1(buffer, lock, max, bufferIsEmpty));
		}
		for(int i = 0; i < cpu; i++) {
			try {
				result += ecs.take().get(); //aggiorno result
			} catch(InterruptedException e) {
				e.printStackTrace();
			} catch (ExecutionException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		executor.shutdown(); //smetto di prendere  i task
		return result;
	}

}
