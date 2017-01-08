package processor;

public class FakeProcessor implements OnerousProcessor {
	
	public final static int MIN_COUNT = 1000;
	private int max;
	private java.util.Random random;

	public FakeProcessor(int max) {
		// TODO Auto-generated constructor stub
		this.max = max;
		this.random = new java.util.Random();
	}

	@Override
	public int onerousFunction(int value) {
		// TODO Auto-generated method stub
		int r = this.random.nextInt(this.max);
		int n = Math.max(MIN_COUNT, r); //non meno di MIN_COUNT
		for(int counter = 0; counter<n; counter++) {
			r = this.random.nextInt(this.max);
			r = r ^ this.random.nextInt(this.max);
			r = r ^ this.random.nextInt(this.max);
		}
		return value;
	}

}
