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
        Thread atender;
        Socket server;
        bool molestar = true;
        string nombre_usuario;
        int nombrepersonamensajeada;
        string listajugadorespartida;
        int dineroapostado;
        string colorapostado;
        bool jugadarealizada = false;
        bool partidaempezada = false;
        bool owner = false;
        string[] trozos3 = new string[10];
        string[] lista_de_jugadas = new string[10];
        int numero_de_jugadores =0;
        int idpartida;
        int numero_de_jugadores_inicial;
        bool ganador = false;
        bool empezarpartida = false;
        //textBox1.Visible = false;

        delegate void DelegadoParaActualizarLista(string[] trozos);
        bool conectado = false;

        public Form1()
        {
            //Decidimos lo que queremos que se vea al iniciar el programa.
            InitializeComponent();
            pictureBox1.Visible = false;
            dataGridView3.Visible = false;
            button11.Visible = false;
            label16.Visible = false;
            textBox6.Text = "50081";
            textbox_IP.Text = "147.83.117.22";

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }
        //Atendemos los mensajes que recibimos del servidor para saber que message box hay que poner en cada uno de los casos.
        private void atender_mensajes_servidor()
        {
            while (true)
            {
                byte[] msg = new byte[80];

                // recibo mensaje del servidor
                if (conectado == false)
                {
                    server.Shutdown(SocketShutdown.Both);
                    server.Close();
                    break;
                }

                else
                {
                    server.Receive(msg);

                    string mensaje = Encoding.ASCII.GetString(msg);
                    mensaje = mensaje.TrimEnd('\0');  //Limpia el mensaje del servidor
                    string[] trozos = mensaje.Split('/');

                    // Averiguo el tipo de mensaje
                    if (trozos[0] == "0")   //Actualiza la lista de conectados
                    {
                        if (Convert.ToInt32(trozos[1]) > 0)
                        {
                            dataGridView1.Invoke(new DelegadoParaActualizarLista(Escribir_grid1), new object[] { trozos });
                            dataGridView2.Invoke(new DelegadoParaActualizarLista(Escribir_grid2), new object[] { trozos });
                        }
                    }

                    if (trozos[0] == "20")
                    {
                        MessageBox.Show("Se ha desregistrado correctamente");
                    }
                    if (trozos[0] == "-20")
                    {
                        MessageBox.Show("No se ha podido desregistrarse");
                    }
                    if (trozos[0] == "1")
                    {
                        MessageBox.Show("Registro ok");
                    }
                    if (trozos[0] == "-1")
                    {
                        MessageBox.Show("Registro fallido");
                    }
                    if (trozos[0] == "2")
                    {
                        MessageBox.Show("Login ok");

                    }
                    if (trozos[0] == "-2")
                    {
                        MessageBox.Show("Login fallido");
                    }
                    if (trozos[0] == "3")
                    {
                        MessageBox.Show("el jugador ha ganado:" + trozos[1] + "partidas");
                    }
                    if (trozos[0] == "-3")
                    {
                        MessageBox.Show("Peticion fallida");
                    }
                    if (trozos[0] == "4")
                    {
                        MessageBox.Show("han jugado:" + trozos[1] + " partidas");
                    }
                    if (trozos[0] == "-4")
                    {
                        MessageBox.Show("Peticion fallida");
                    }
                    if (trozos[0] == "5")
                    {
                        MessageBox.Show("hay:" + trozos[1] + "jugadores registrados");
                    }
                    if (trozos[0] == "-5")
                    {
                        MessageBox.Show("Peticion fallida");
                    }
                    if (trozos[0] == "6")
                    {
                        string confirmacion;
                        if (empezarpartida == false)
                        {
                            DialogResult dialogresult = MessageBox.Show(trozos[1] + " quiere jugar contigo", "Invitación de partida", MessageBoxButtons.OKCancel);
                            if (dialogresult == DialogResult.OK)
                            {
                                confirmacion = "0";
                                empezarpartida = true;
                            }
                            else if (dialogresult == DialogResult.Cancel)
                                confirmacion = "1";

                            else
                                confirmacion = "3";
                        }

                        else
                        {
                            confirmacion = "3";
                        }

                        string respuesta = "-6/" + confirmacion + "/" + trozos[1];
                        byte[] code = System.Text.Encoding.ASCII.GetBytes(respuesta);
                        server.Send(code);
                    }
                    if (trozos[0] == "-6")  //Respuesta de invitación
                    {
                        if (trozos[1] == "0")
                        {
                            MessageBox.Show("Invitación aceptada por " + trozos[2]);
                            Escribir_lista(trozos[2]);
                            partidaempezada = true;
                            owner = true;
                            empezarpartida = true;

                        }

                        if (trozos[1] == "1")
                        {
                            MessageBox.Show("Invitación rechazada");
                        }

                        if (trozos[1] == "3")
                        {
                            MessageBox.Show(trozos[2] + " está en otra partida en curso");
                        }
                    }
                    if (trozos[0] == "7")
                    {
                        listBox1.Invoke(new DelegadoParaActualizarLista(Escribir_listgrid), new object[] { trozos });
                    }
                   

                }

            }
        }
        //Cuando un jugador apuesta un dinero y apuesta a un color se realiza esta función que le dice si ha ganado o a perdido su apuesta.
       

        //DataGridView con los Usuarios conectados
        private void Escribir_grid1(string[] trozos)
        {
            int n = Convert.ToInt32(trozos[1]);
            dataGridView1.RowCount = n;
            for (int i = 0; i < n; i++)
            {
                trozos[i + 2] = trozos[i + 2].TrimEnd('0');
                dataGridView1.Rows[i].Cells[0].Value = trozos[i + 2];
            }
        }

        //DataGridView con los Usuarios Chat
        private void Escribir_grid2(string[] trozos)
        {
            int n = Convert.ToInt32(trozos[1]);

            dataGridView2.RowCount = n;
            for (int i = 0; i < n; i++)
            {
                trozos[i + 2] = trozos[i + 2].TrimEnd('0');
                dataGridView2.Rows[i].Cells[0].Value = trozos[i + 2];
            }
        }

        //DataGridView con laTabla de dinero
        private void Escribir_grid3(string[] trozos)
        {
            int n = trozos.Length;
            numero_de_jugadores_inicial = numero_de_jugadores;
            dataGridView3.RowCount = n+1;
            for (int g = 0; g < n; g++)
            {
                trozos[g] = trozos[g].TrimEnd('0');
                dataGridView3.Rows[g].Cells[0].Value = trozos[g];
                dataGridView3.Rows[g].Cells[1].Value = "50";
            }
        }

        //DataGridView con la Tabla dinero
        private void Escribir_grid4(string[] trozos)
        {
            int n = 0;
            numero_de_jugadores_inicial = numero_de_jugadores;
            while(trozos[n] != null)
            {
                n++;
            }
            dataGridView3.RowCount = n+1;
            for (int g = 0; g < n; g++)
            {
                trozos[g] = trozos[g].TrimEnd('0');
                dataGridView3.Rows[g].Cells[0].Value = trozos[g];
                dataGridView3.Rows[g].Cells[1].Value = "50";
            }
        }

        //DataGridView con los usuarios conectados
        private void Escribir_lista(string nombre)
        {
            listajugadorespartida = listajugadorespartida + nombre + "/";
            numero_de_jugadores++;
        }

        // DataGridView que no molesten cuado clicas el botón
        private void Escribir_listgrid(string[] trozos)
        {
            if (molestar == true)
            {

                string mensaje = Convert.ToString(trozos[1]) + ": " + Convert.ToString(trozos[2]);
                listBox1.Items.Add(mensaje);

            }

        }


        private void Button2_Click(object sender, EventArgs e) //Registrarse
        {
            // Envia el nombre y el password del registro con el código 1 y separado por /
            string mensaje = "1/" + Convert.ToString(nombre_registro.Text) + "/" + Convert.ToString(password_registro.Text);
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            nombre_registro.Clear();
            password_registro.Clear();

        }

        private void Button3_Click(object sender, EventArgs e) //Desconectar
        {
            // Se terminó el servicio. 
            // Nos desconectamos

            this.BackColor = Color.Gray;
            string mensaje = "0/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            server.Shutdown(SocketShutdown.Both);
            atender.Abort();
            server.Close();

        }

        private void Button1_Click_1(object sender, EventArgs e)  //conectar servidor
        {
            IPAddress direc = IPAddress.Parse("192.168.56.101");
            IPEndPoint ipep = new IPEndPoint(direc, Convert.ToInt32(textBox6.Text));
            this.BackColor = Color.Green;

            //Creamos el socket 
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);//Intentamos conectar el socket
                ThreadStart ts = delegate { atender_mensajes_servidor(); };
                atender = new Thread(ts);
                atender.Start();
                conectado = true;
                
            }

            catch (SocketException)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return 
                MessageBox.Show("No he podido conectar con el servidor");
                return;
            }
            
        }

        private void Button_login_Click(object sender, EventArgs e) //login
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

        private void button2_Click_1(object sender, EventArgs e) //partidas ganadas por un jugador
        {
            string mensaje = "3/" + textBox2.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            textBox2.Clear();
        }

        private void button5_Click(object sender, EventArgs e) //numero jugadores registrados
        {
            string mensaje = "5/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);



        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e) //invitar
        {
            if (nombre_usuario == dataGridView1.CurrentRow.Cells[0].Value.ToString())
            {
                MessageBox.Show("No te puedes invitar a ti mismo. Lo sentimos ^^");
            }
            else
            {
                int invitado = e.RowIndex;
                string mensaje = "6/" + nombre_usuario + "/" + invitado;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }

        }

        private void dataGridView2_CellClick(object sender, DataGridViewCellEventArgs e) //seleccionar persona para mensaje
        {
            nombrepersonamensajeada = e.RowIndex;
        }

        private void button6_Click(object sender, EventArgs e) //enviar mensaje
        {
            if (nombre_usuario == dataGridView2.CurrentRow.Cells[0].Value.ToString())
            {
                MessageBox.Show("No te puedes hablar a ti mismo. Lo sentimos :(");
            }
            else
            {
                string mensaje = "7/" + nombre_usuario + "/" + nombrepersonamensajeada + "/" + Convert.ToString(textBox4.Text);
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
                textBox4.Clear();
            }
        }

        private void button7_Click(object sender, EventArgs e) //botón de no molestar cuando lo clicas
        {
            if (molestar == false)
            {
                this.button7.BackColor = Color.Gray;
                button7.Text = "No ocupado";
                molestar = true;
            }
            else
            {
                this.button7.BackColor = Color.Red;
                button7.Text = "Ocupado";
                molestar = false;
            }

        }
        //Cuando iniciamos la partida hay objetos que desaparecen y otro que se empezaran a ver, igual que ha pasado al principio
        //del codigo pero en este caso serán los opuestos.
        private void Pasar_a_Partida()
        {
            button1.Visible = false;
            textbox_IP.Visible = false;
            groupBox1.Visible = false;
            groupBox2.Visible = false;
            button3.Visible = false;
            dataGridView1.Visible = false;
            label5.Visible = false;
            textBox2.Visible = false;
            button2.Visible = false;
            label7.Visible = false;
            button5.Visible = false;
            label8.Visible = false;
            button7.Visible = false;
            dataGridView2.Visible = false;
            listBox1.Visible = false;
            label9.Visible = false;
            textBox4.Visible = false;
            button6.Visible = false;
            pictureBox1.Visible = true;
            dataGridView3.Visible = true;
            button11.Visible = false;
            textBox6.Visible = false;
            label13.Visible = false;
            label14.Visible = false;
            label15.Visible = false;
            label16.Visible = true;
            button10.Visible = false;
        }

     


        private void DesregistrarseButton_Click(object sender, EventArgs e)//Desregistrarse de la base de datos
        {
            //Nos desregistraremos de la base de datos
            string mensaje = "20/" + textBox1.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            textBox1.Clear();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            //Nos desregistraremos de la base de datos
            string mensaje = "21/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);
            textBox1.Clear();
        }

       

        
    }
}