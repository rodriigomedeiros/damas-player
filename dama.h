#ifndef DAMA_H
#define DAMA_H

/* Macros para as imagens usadas ******************************************************************************************/
#define PECA_VERDE 		"imagens/pecaV.gif"
#define PECA_VERDE_CLICKADA	"imagens/pecaVClicked.gif"
#define PECA_AMARELA		"imagens/pecaA.gif"
#define PECA_AMARELA_CLICKADA	"imagens/pecaAClicked.gif"
#define DAMA_VERDE		"imagens/damaV.gif"
#define DAMA_VERDE_CLICKADA	"imagens/damaVClicked.gif"
#define DAMA_AMARELA		"imagens/damaA.gif"
#define DAMA_AMARELA_CLICKADA	"imagens/damaAClicked.gif"
#define BOX_PRETO		"imagens/boxBlack.gif"
#define BOX_BRANCO		"imagens/boxWhite.gif"
#define BOX_POSSIVEL_JOGADA	"imagens/boxJogadaPossivel.gif"
#define ICONE			"imagens/icone.png"
#define SCREEN_GAME		"imagens/DamasPlayer.gif"

#include<gtk/gtk.h>
		struct peca{
		  int x, y;
		  GtkWidget *imagem, *event_box, *box;
		  struct table *referencia;
		};
		struct table{
			int tipo[8][8], vez, xJob[3], yJob[3], job;
			gint quant_pecasVerdes, quant_pecasAmarelas;
			GtkWidget *tabela, *statusbar;
			struct peca *referenciadapeca[8][8];
			gchar jogador1[20], jogador2[20];
		};
		void sairF(GtkWidget *, gpointer);
		void sobreF(GtkWidget *, gpointer);
		void geraTabuleiro(GtkWidget *);
		void criaMenu(GtkWidget *, gpointer);
		void Vencedor(gchar *);
		void clickou(GtkWidget *, gpointer, gpointer *);
		void PopUp(gchar *, gchar *, gint);
		void regrasF(GtkWidget *, gpointer);
		void NovoJogoF(GtkWidget *, gpointer);
		void salvaJogo(GtkWidget *, gpointer);
		void carregaJogoF(GtkWidget *, gpointer);
		void NovoJogoQ(GtkWidget *, gpointer);
		void testaJogo(gpointer *);
		void limpaPJogadas(gpointer *);
		void possivelJogadaDama(gpointer, int, int, int);
		void gravaNomesJogadores(gpointer);
		void jogadorStatusbar(gpointer , gpointer );
		GdkPixbuf *criaIcone(const gchar *);
#endif
