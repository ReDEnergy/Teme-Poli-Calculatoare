import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.EOFException;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;

public class ClientStub {
	public static byte[] readMessage(DataInputStream in) {
		byte[] message = null;
		// There's no way in java to know if the socket closed on the other side
		// We can only read something and see if it throws a java.io.EOFException
		try {
			byte size = in.readByte();
			message = new byte[size];
			in.readFully(message);
		} catch (EOFException e) {
			return null;
		} catch (IOException e) {
			e.printStackTrace();
			return null;
		}
		return message;
	}
	
	public static void sendMessage(final byte[] message, DataOutputStream out) {
		byte size = (byte) message.length;
		try {
			out.writeByte(size);
			out.write(message);
		} catch (SocketException e) {
			return;
		} catch (IOException e) {
			e.printStackTrace();
			return;
		}
	}
	
	public static void main(String args[]) {
		if (args.length < 4) {
			System.out.println("Usage: ./client server_hostname server_port opponent_level(1=dumb, 5, 7, 8) own_level(1=dumb, 5, 7, 8)");
			return;
		}
		// variabile pentru conexiune
		Socket socket = null;
		DataOutputStream out = null;
        DataInputStream in = null;

		try {
			// realizez conexiunea la server
			socket = new Socket(args[0], Integer.parseInt(args[1]));
			// scriu in out pe socket
			out = new DataOutputStream(socket.getOutputStream());
			// citesc din in de pe socket
            in = new DataInputStream(socket.getInputStream());
            
            // trimit primul mesaj - dificulatea adversarului
            byte[] message = new byte[1];
            message[0] = Byte.parseByte(args[2]);
            ConnectionExample.sendMessage(message, out);
            
            // primesc raspuns cu culoarea mea
            message = ConnectionExample.readMessage(in);
            if (message[0] == 1) {
            	// sunt jucatorul negru
            } else if (message[0] == 0) {
            	// sunt jucatorul alb
            } else {
            	// mesaj invalid; eroare!
            }

            while (true) {
            	message = ConnectionExample.readMessage(in);
            	// Primesti un mesaj de la server care contine miscarile adversarului si zarul tau 
            	// Daca sunteti primii la mutare, atunci nu primiti mutari din partea adversarului
            	// Daca adversarul nu a putut sa mute, atunci nu primiti mutari din partea adversarului
            	
            	// la un moment dat jocul se va termina
            	boolean gameOver = true;
            	if (gameOver){
            		break;
            	}
            	
            	// Compune mesajul tau; dimensiunea lui depinde de ceea ce vei trimite
            	byte[] yourResponse = new byte[1];
            	ConnectionExample.sendMessage(yourResponse, out);
            }
            socket.close();
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println(e.getMessage());
		}
	}
}
