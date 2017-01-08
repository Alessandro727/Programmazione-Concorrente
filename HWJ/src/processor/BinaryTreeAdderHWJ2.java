package processor;

import domain.Node;

import java.util.ArrayList;
import java.util.Deque;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CompletionService;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorCompletionService;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class BinaryTreeAdderHWJ2 implements BinaryTreeAdder {
	private Map<Integer,Deque<Node>> buffers;
	private Lock stealWorkLock;
	private List<Lock> buffersLock;
	int cpu;
	int max;
	

	public BinaryTreeAdderHWJ2(int cpu, int max) {
		// TODO Auto-generated constructor stub
		this.buffers = new HashMap<Integer, Deque<Node>>();
		this.stealWorkLock = new ReentrantLock();
		this.buffersLock = new ArrayList<Lock>();
		this.cpu = cpu;
		this.max = max;
	}

	@Override
	public int computeOnerousSum(Node root) {
		// TODO Auto-generated method stub
		boolean bufferIsEmpty = false;
		if(root == null) {
			return 0;
		}
		int result = 0;
		ExecutorService executor = Executors.newFixedThreadPool(cpu);
		// con ecs gestisco i task generati processando in ordine di completamento e non di creazione
		CompletionService<Integer> ecs = new ExecutorCompletionService<Integer>(executor); 
		for(int i = 0; i<cpu; i++) {
			Deque<Node> buffer = new LinkedList<Node>();
			this.buffers.put(i, buffer);
			Lock bufferLock = new ReentrantLock();
			this.buffersLock.add(bufferLock);
		}
		this.buffers.get(0).offerFirst(root);
		for(int i = 0; i < cpu; i++) {
			ecs.submit(new generateTaskHWJ2(buffers, i, max, cpu, bufferIsEmpty, buffersLock, stealWorkLock));
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

