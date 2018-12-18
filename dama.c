#include <gtk/gtk.h>
#include <stdlib.h>
#include <time.h>
#include "dama.h"

/* Início da Bagaça */
int main(int argc, char *argv[])
{
    struct table *tab;
    tab = (struct table *)malloc(sizeof(struct table));
    int a, b;
    GtkWidget *window, *caixaMain;
    gtk_init (&argc, &argv);
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Damas Player - v1.0");
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 1);
    caixaMain = gtk_vbox_new(FALSE, 2); 					//Cria Caixa Principal
    gtk_container_add(GTK_CONTAINER(window), caixaMain); 			// Adiciona a caixa principal a Window.
    criaMenu(caixaMain, tab);				 			// Cria Menu
    tab->statusbar = gtk_statusbar_new();
    gtk_box_pack_end(GTK_BOX(caixaMain), tab->statusbar, FALSE, TRUE, 1);


    
/* Gera o Tabuleiro ******************/
    
    tab->tabela = gtk_table_new(8, 8, TRUE);
    tab->quant_pecasAmarelas = 12;
    tab->quant_pecasVerdes = 12;
    tab->vez = 1;
    tab->job = 0;
    
    gtk_table_set_row_spacings(GTK_TABLE(tab->tabela), 0);
    gtk_table_set_col_spacings(GTK_TABLE(tab->tabela), 0);

    int i=0, j=0, pecasV=0, pecasA=0, casas=0;
     for(i=0; i < 8; i++){
	for(j=0; j < 8; j++){
	  struct peca *ref;
	  ref = (struct peca *)malloc(sizeof(struct peca));
	  if((i-j) % 2 != 0){
	    if(pecasV < 12){ 
	      ref->box = gtk_image_new_from_file(PECA_VERDE);
	      pecasV++;
	      tab->tipo[i][j] = 1;
	    } else if ((casas >= 40) && (pecasA < 12)){
	      ref->box = gtk_image_new_from_file(PECA_AMARELA);
	      pecasA++;
	      tab->tipo[i][j] = 2;
	    } else {
	      ref->box = gtk_image_new_from_file(BOX_PRETO);
	      pecasV++;
	      tab->tipo[i][j] = 0;
	    }
	    /* Insere peças que recebem clicks (verdes, amarelas e vazias pretas) */
	    ref->x = i;
	    ref->y = j;
	    ref->referencia = tab;
	    ref->imagem = ref->box;
	    ref->event_box = gtk_event_box_new ();
	    gtk_container_add(GTK_CONTAINER(ref->event_box), ref->box);
	    gtk_table_attach_defaults(GTK_TABLE(tab->tabela), ref->event_box, j, j+1, i, i+1 );
	    g_signal_connect (G_OBJECT(ref->event_box), "button_press_event", GTK_SIGNAL_FUNC(clickou), (gpointer)ref);
	    /* Fim do insere peças que recebem clicks */
	    
	  } else {
	    ref->box = gtk_image_new_from_file(BOX_BRANCO);
	    ref->event_box = gtk_event_box_new ();
	    tab->tipo[i][j] = -1;
	    gtk_container_add(GTK_CONTAINER(ref->event_box), ref->box);
	    gtk_table_attach_defaults(GTK_TABLE(tab->tabela), ref->event_box, j, j+1, i, i+1 );
	  }
	  casas++;
	  tab->referenciadapeca[i][j] = ref;
	}
     }
     for(a=0; a<3; a++){
	tab->xJob[a] = -1;
	tab->yJob[a] = -1;
      }

    gtk_container_add(GTK_CONTAINER(caixaMain), tab->tabela);
    gtk_window_set_icon(GTK_WINDOW(window), criaIcone("imagens/icone.png"));	// Exibir o ícone
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    g_signal_connect_swapped(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), G_OBJECT(window));
    
    gtk_widget_show_all(window);
    gravaNomesJogadores(tab);
    jogadorStatusbar(tab, NULL);
    gtk_main();
    
    return 0;
}
