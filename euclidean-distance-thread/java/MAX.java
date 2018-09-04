public class MAX implements Runnable{
	static final int MAX_THREADS = 8;
	static final int N = 10000000;
	private double[] maior;
	private int threadID;
	private double[] result;

	public MAX(int threadID, double[] result, double[] maior){
		this.threadID = threadID;
		this.result = result;
		this.maior = maior;
	}

	public void run(){
		maior[this.threadID] = result[this.threadID];
		for(int i = this.threadID; i < N; i+= MAX_THREADS){
			//result[i] = Math.sqrt((Math.pow(r1[i],2) + Math.pow(r2[i],2) - 2*r1[i]*r2[i]*Math.cos(teta1[i]-teta2[i])));
			if(result[i] > maior[this.threadID])
				maior[this.threadID] = result[i];
		}
	}
}