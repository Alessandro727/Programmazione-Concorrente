package test;

import static org.junit.Assert.assertEquals;

import org.junit.Before;
import org.junit.Test;

import domain.BinaryTreeGenerator;
import domain.CountBinaryTree;
import domain.Node;
import processor.BinaryTreeAdder;
import processor.BinaryTreeAdderHWJ1;
import processor.BinaryTreeAdderHWJ2;

public class TestHWJ3 {
	BinaryTreeGenerator tree;
	int cpu;
	CountBinaryTree countTree;
	BinaryTreeAdder adderTree;
	
	@Before
	public void setUp() {
		this.tree = new BinaryTreeGenerator();
		this.cpu = Runtime.getRuntime().availableProcessors();
		this.countTree = new CountBinaryTree();
		this.adderTree = null;
	}

	@Test
	public void testEmptyTree() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		int result = adderTree.computeOnerousSum(null);
		assertEquals(result, 0);
	}
	
	@Test
	public void test_binaryTreeBalanced_depth0() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createBalancedBinaryTree(0);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeBalanced(0);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeNotBalanced_depth0() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createNotBalancedBinaryTree(0,0);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeNotBalanced(0, 0);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeBalanced_depth1() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createBalancedBinaryTree(1);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeBalanced(1);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeNotBalanced_depth1() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createNotBalancedBinaryTree(1,0);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeNotBalanced(1, 0);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeBalanced_depth10() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createBalancedBinaryTree(10);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeBalanced(10);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeNotBalanced_depth10() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createNotBalancedBinaryTree(10,8);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeNotBalanced(10, 8);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeNotBalancedStrongly_depth10() {
		adderTree = new BinaryTreeAdderHWJ1(cpu, 2000);
		Node root = tree.createNotBalancedBinaryTree(0,10);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeNotBalanced(0, 10);
		assertEquals(result, expected);
	}
	
	@Test
	public void test_binaryTreeNotBalanced_depth20() {
		adderTree = new BinaryTreeAdderHWJ2(cpu, 2000);
		Node root = tree.createNotBalancedBinaryTree(20,10);
		int result = adderTree.computeOnerousSum(root);
		int expected = countTree.countBinaryTreeNotBalanced(20, 10);
		assertEquals(result, expected);
	}

}
