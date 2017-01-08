package domain;

public class NodeImpl implements Node {
	private int value;
	private Node dx;
	private Node sx;
	
	public NodeImpl(int value, Node dx, Node sx) {
		this.value = value;
		this.sx = sx;
		this.dx = dx;
	}

	@Override
	public Node getSx() {
		// TODO Auto-generated method stub
		return this.sx;
	}

	@Override
	public Node getDx() {
		// TODO Auto-generated method stub
		return this.dx;
	}

	@Override
	public int getValue() {
		// TODO Auto-generated method stub
		return this.value;	
	}

}
