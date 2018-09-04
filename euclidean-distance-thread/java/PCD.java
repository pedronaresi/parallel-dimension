import java.util.Random;

public class PCD implements Runnable{
	static final int MAX_THREADS = 8;
	static final int N = 10000000;
	private int threadID;
	private double[] result;
	private double[] r1, teta1;
	private double[] r2, teta2;
	public PCD(int threadID, double[] result, double[] r1, double[] teta1, double[] r2, double[] teta2){
		this.threadID = threadID;
		this.result = result;
		this.r1 = r1;
		this.r2 = r2;
		this.teta1 = teta1;
		this.teta2 = teta2;
	}
	public void run(){
		for(int i = this.threadID; i < N; i+= MAX_THREADS){
			result[i] = Math.sqrt((Math.pow(r1[i],2) + Math.pow(r2[i],2) - 2*r1[i]*r2[i]*Math.cos(teta1[i]-teta2[i])));
		}
	}
	public static void main(String... args) throws InterruptedException{
		
		Random rnd = new Random();
		Thread[] threads = new Thread[MAX_THREADS];
		double[] r1, teta1;
		double[] r2, teta2;
		double[] result;
		double[] maior;

		result = new double[N];
		maior = new double[MAX_THREADS];
		r1 = new double[N];
		teta1 = new double[N];
		r2 = new double[N];
		teta2 = new double[N];
		for(int i = 0; i < N; i++){
			r1[i] = rnd.nextDouble() * 100;
			teta1[i] = rnd.nextDouble() * 360;
			r2[i] = rnd.nextDouble() * 100;
			teta2[i] = rnd.nextDouble() * 360;
		}
		long startTime = System.currentTimeMillis();
		for(int i = 0; i < threads.length; i++){
			threads[i] = new Thread(new PCD(i, result, r1, teta1, r2, teta2));
			threads[i].start();
		}

		for (Thread t : threads) {
			t.join();
		}

		//Calculo da maior distancia
		long totalTime = System.currentTimeMillis() - startTime;
		System.out.println("Tempo decorrido: " + totalTime + "ms");

		startTime = System.currentTimeMillis();
		for(int i = 0; i < threads.length; i++){
			threads[i] = new Thread(new MAX(i, result, maior));
			threads[i].start();
		}

		for (Thread t : threads) {
			t.join();
		}
		totalTime = System.currentTimeMillis() - startTime;
		System.out.println("Tempo decorrido do calculo do maior: " + totalTime + "ms");

	}
}