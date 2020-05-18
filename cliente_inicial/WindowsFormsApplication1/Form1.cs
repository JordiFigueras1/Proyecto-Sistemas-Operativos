using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender;
        delegate void DelegadoParaLimpiar();
        delegate void DelegadoParaEscribir(string[] trozos);
        String nombre_usuario;
        int confirmacion = 5;
        int usuariomensaje;

        public Form1()
        {
            InitializeComponent();
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           
        }
        private void EscribirLista(string[] trozos)
        {
                int f = Convert.ToInt32(trozos[2]);
                int i = 0;
                if (dataGridView1.Rows.Count != 0)
                    dataGridView1.Rows.Clear();
                while (i < f)
                {
                    dataGridView1.Rows.Add(trozos[i + 3]);
                    i++;
                }
        }
        private void EscribirLista2(string[] trozos)
        {
            dataGridView2.Rows.Add(trozos[2]);
        }

        private void Escribir_listgrid(string[] trozos)
        {
            {
                int f = Convert.ToInt32(trozos[2]);
                int i = 0;
                if (dataGridView3.Rows.Count != 0)
                    dataGridView3.Rows.Clear();
                while (i < f)
                {
                    dataGridView3.Rows.Add(trozos[i + 3]);
                    i++;
                }

                
            }

        }

        private void Escribirchat(string[] trozos)

        {
            string mensaje = Convert.ToString(trozos[1]) + ":" + Convert.ToString(trozos[3]);
        CHAT.Items.Add(mensaje);
        }

 
        private void botondesc_Click(object sender, EventArgs e)
        {
           // Se terminó el servicio. 
                // Nos desconectamos
            this.BackgroundImage = disabled;
                dataGridView1.Rows.Clear();
                string mensaje = "0/";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                this.Text = "Form1";
                server.Send(msg);
                atender.Abort();
                server.Shutdown(SocketShutdown.Both);
                server.Close();

        }

        private void botonconectar_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("192.168.56.101");
            IPEndPoint ipep = new IPEndPoint(direc, 9060);


            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                this.BackColor = Color.Green;
            }

            catch (SocketException)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }

            ThreadStart ts = delegate { AtenderServidor(); };
            atender = new Thread(ts);
            atender.Start();
        }

        private void Button_login_Click(object sender, EventArgs e)
        {
            // Envia el nombre y el password del login con el código 2 y separado por /
            nombre_usuario = nombre_login.Text;
            string mensaje = "2/" + Convert.ToString(nombre_login.Text) + "/" + Convert.ToString(password_login.Text);
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            this.Text = nombre_usuario;
            nombre_login.Clear();
            password_login.Clear();
        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            string mensaje = "3/"+textBox2.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            textBox2.Clear();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string mensaje = "4/" + Convert.ToString(textBox1.Text) +"/"+ Convert.ToString(textBox3.Text) ;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            textBox1.Clear();
            textBox3.Clear();
        }

        

        private void button5_Click(object sender, EventArgs e)
        {
            string mensaje = "5/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            string nombredelgrid = dataGridView1.CurrentRow.Cells[0].Value.ToString();
            nombredelgrid = nombredelgrid.TrimEnd('\0');
            if (nombre_usuario == nombredelgrid)
            {
                MessageBox.Show("No te puedes invitar a ti mismo");
            }
            else
            {
                string mensaje = "7/" + nombre_usuario + "/" + nombredelgrid;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
        }

        private void dataGridView2_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            int rowIndex = dataGridView2.CurrentCell.RowIndex;
            dataGridView2.Rows.RemoveAt(rowIndex);
        }

        private void button6_Click(object sender, EventArgs e)
        {
            int n = dataGridView2.RowCount;
            if (n==0)
            {
                MessageBox.Show("No has seleccionado a nadie para invitar");
            }
            else
            {
                int k = 0;
                while (k<n)
                {
                    string nombresdelgrid = dataGridView2.Rows[k].Cells[0].Value.ToString();
                    nombresdelgrid = nombresdelgrid.TrimEnd('\0');
                    string mensaje = "8/" + nombresdelgrid + "/" + nombre_usuario + "/" + n; ;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                    k++;
                    
                }
            }
        }
        private void AtenderServidor()
        {
            while(true)
            {
                //Recibimos mensaje del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                string [] trozos = Encoding.ASCII.GetString(msg2).Split('/');
                int codigo = Convert.ToInt32(trozos[0]);
                switch (codigo)
                {
                    case 1:  //Respuesta del registro
                        
                        if (Convert.ToInt32(trozos[1]) == 1)
                        {
                            MessageBox.Show("Registro ok");
                        }
                        else
                        {
                            MessageBox.Show("Registro fallido");
                        }
                        break;

                    case 2: //Respuesta del login

                        if (Convert.ToInt32(trozos[1]) == 2)
                        {
                            MessageBox.Show("login ok");
                        }
                        else
                        {
                            MessageBox.Show("login fallido");
                        }
                        
                        break;

                    case 3: //Respuesta de cuantas partidas ha ganado un jugador

                        if (Convert.ToInt32(trozos[1]) == 3)
                        {
                            MessageBox.Show("el jugador ha ganado:" + Convert.ToInt32(trozos[2]) + "partidas");
                        }
                        else
                        {
                            MessageBox.Show("peticion fallida");
                        }
                        break;

                    case 4: //Respuesta de cuantas partidas han jugado entre dos jugadores
                       
                        if (Convert.ToInt32(trozos [1]) == 4)
                        {
                            MessageBox.Show("han jugado:" + Convert.ToInt32(trozos[2]) + " partidas");
                        }
                        else 
                        { 
                            MessageBox.Show("peticion fallida"); 
                        }
                        break;

                    case 5: //Respuesta a cuantos jugadores hay registrados

                        if (Convert.ToInt32(trozos[1]) == 5)
                        {
                            MessageBox.Show("hay:" + Convert.ToInt32(trozos[2]) + "jugadores registrados");
                        }
                        else 
                        { 
                            MessageBox.Show("peticion fallida"); 
                        }
                        break;

                    case 6: //Enviamos la lista a todos los usuarios conectados

                        if (Convert.ToInt32(trozos[1]) == 6)
                        {
                            dataGridView1.Invoke(new DelegadoParaEscribir(EscribirLista), new object[] { trozos });
                            dataGridView3.Invoke(new DelegadoParaEscribir(Escribir_listgrid), new object[] { trozos });
                            
                        }
                        else
                        {
                            MessageBox.Show("peticion fallida");
                        }
                        
                        break;

                    case 7: //Recibimos la invitacion del invitador

                        if (Convert.ToInt32(trozos[1]) == 7)
                        {

                            dataGridView2.Invoke(new DelegadoParaEscribir(EscribirLista2), new object[] { trozos });
                                  

                        }
                        break;

                    case 8: //Recibimos los sockets de los que han aceptado la invitacion y les enviamos que empieza la partida o no

                        trozos[3] = trozos[3].TrimEnd('\0');
                        DialogResult dialogresult = MessageBox.Show(trozos[2] + " quiere jugar contigo", "Invitación de partida", MessageBoxButtons.OKCancel);
                        if (dialogresult == DialogResult.OK)
                        {
                            string invitado;
                            invitado = trozos[1];
                            confirmacion = 1;
                            int n = Convert.ToInt32(trozos[3]);
                            string mensaje = "9/" + invitado + "/" + confirmacion + "/" + n;
                            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                            server.Send(msg);
                        }
                        else
                        {
                            string invitado;
                            invitado = trozos[1];
                            confirmacion = 0;
                            string mensaje = "9/" + invitado + "/" + confirmacion;
                            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                            server.Send(msg);
                        }

                        
                        break;
                    case 9:

                        MessageBox.Show("Todos los Jugadors han aceptado la partida");
                        break;

                    case 11:
                        if (trozos[0] == "11")
                        {
                            CHAT.Invoke(new DelegadoParaEscribir(Escribirchat), new object[] { trozos });
                            
                        }
                        else
                        {
                            MessageBox.Show("peticion fallida");
                        }

                        break;
                }

            }
        }

        private void Button2_Click(object sender, EventArgs e)
        {
            // Envia el nombre y el password del registro con el código 1 y separado por /
            string mensaje = "1/"+Convert.ToString (nombre_registro.Text)+"/"+Convert.ToString (password_registro.Text);
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            nombre_registro.Clear();
            password_registro.Clear();
        }

        public Image disabled { get; set; }


        private void dataGridView3_CellClick(object sender, DataGridViewCellEventArgs e) //seleccionar persona para enviar el mensaje
        {
            if (e.RowIndex == -1)
            { MessageBox.Show(" error en la seleccion"); }
            else
            usuariomensaje = e.RowIndex;
            
        }
        
        private void buttonchat_Click(object sender, EventArgs e)
        {
           
            if (System.String.Compare(dataGridView3.CurrentRow.Cells[usuariomensaje].Value.ToString(), nombre_usuario)==0)
            {   
                
                MessageBox.Show("No te puedes hablar a ti mismo. Lo sentimos :(");
            }
            else
            {
                string mensaje = "11/" + nombre_usuario + "/" + dataGridView3.CurrentRow.Cells[usuariomensaje].Value.ToString() + "/" + Convert.ToString(textBoxChat.Text);
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                    textBoxChat.Clear();
                
        
        } 
        }


    }
    }

