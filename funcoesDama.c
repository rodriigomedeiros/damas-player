#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "dama.h"



/* Icone do programa *****************************************************************************************************/
GdkPixbuf *criaIcone(const gchar * filename)
{
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   return pixbuf;
}

/* Função Sair ************************************************************************************************************/
void sairF(GtkWidget *w, gpointer p){
	gtk_main_quit();
}

/* Sobre o DamasPlayer ***************************************************************************************************/
void sobreF(GtkWidget *w,gpointer p){
  
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file(SCREEN_GAME, NULL);
	GtkWidget *dialog = gtk_about_dialog_new();
	gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(dialog), "Damas Player");
	gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(dialog), "1.0");
	gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(dialog),"Copyright (c) 2010\nRenato Wagner\nRomário Kionys\nRodrigo Medeiros");
	gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(dialog), "Damas Player é um simples jogo de Dama\ndesenvolvido para plataformas GNU/Linux.");
	gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(dialog),    "http://www.rodrigoamedeiros.com");
	gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(dialog), pixbuf);
	g_object_unref(pixbuf), pixbuf = NULL;
	gtk_dialog_run(GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}

/* Cria Menu ************************************************************************************************************/
void criaMenu(GtkWidget *caixaMain, gpointer g)
{
  GtkWidget *menuBar, *arquivo, *ajuda, *sair, *novoJogo, *sobre, *regras, *subMenu, *subMenuAjuda, *salvarJogo, *carregaJogo, *sep;
  menuBar = gtk_menu_bar_new();
  arquivo = gtk_menu_item_new_with_mnemonic("_Jogo");
  ajuda = gtk_menu_item_new_with_mnemonic("Aj_uda");
  
  gtk_menu_append(menuBar, arquivo);
  gtk_menu_append(menuBar, ajuda);
 
  novoJogo = gtk_menu_item_new_with_mnemonic("_Novo Jogo");
  salvarJogo = gtk_menu_item_new_with_mnemonic("_Salvar Jogo");
  carregaJogo = gtk_menu_item_new_with_mnemonic("Abrir _Jogo");
  sair = gtk_menu_item_new_with_mnemonic("_Sair");
  subMenu = gtk_menu_new();
  sep = gtk_separator_menu_item_new();
  
  gtk_menu_append(subMenu, novoJogo);
  gtk_menu_append(subMenu, salvarJogo);
  gtk_menu_append(subMenu, carregaJogo);
  gtk_menu_append(subMenu, sep);
  gtk_menu_append(subMenu, sair);
  
  
  sobre = gtk_menu_item_new_with_mnemonic("_Sobre");
  regras = gtk_menu_item_new_with_mnemonic("_Regras do Jogo");
  subMenuAjuda = gtk_menu_new();
  gtk_menu_append(subMenuAjuda, regras);
  gtk_menu_append(subMenuAjuda, sobre);
  
  /* start conetions with buttons */
  gtk_signal_connect(GTK_OBJECT (novoJogo),"activate",GTK_SIGNAL_FUNC (NovoJogoQ),(gpointer)g);
  gtk_signal_connect(GTK_OBJECT (salvarJogo),"activate",GTK_SIGNAL_FUNC (salvaJogo),(gpointer)g);
  gtk_signal_connect(GTK_OBJECT (carregaJogo),"activate",GTK_SIGNAL_FUNC (carregaJogoF),(gpointer)g);
  gtk_signal_connect(GTK_OBJECT (sobre),"activate",GTK_SIGNAL_FUNC (sobreF),(gpointer)caixaMain);
  gtk_signal_connect(GTK_OBJECT (regras),"activate",GTK_SIGNAL_FUNC (regrasF),(gpointer)NULL);
  gtk_signal_connect(GTK_OBJECT (sair),"activate",GTK_SIGNAL_FUNC (sairF),NULL);
  gtk_signal_connect(GTK_OBJECT (caixaMain), "destroy",GTK_SIGNAL_FUNC (sairF), NULL);
  
  /* end conetions with buttons */
   
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(arquivo), subMenu);
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(ajuda), subMenuAjuda);
  gtk_container_add(GTK_CONTAINER(caixaMain), menuBar);
}

/* Função Clickou *********************************************************************************************************/
void clickou(GtkWidget *w, gpointer n, gpointer *g){
/* 	
      Peças:
	-1 - Branco
	0 - Preto
	1 - Peça Verde
	2 - Peça Amarela
*/

	int pX, pY, pT, vez;
	static int pXAux, pYAux, pTAux, clickouUmaVez=0, pecaLigada = 0;
	GtkImage *image, *imageApaga, *imageJogada;
	static GtkImage *imageAux=NULL;
	int temX = 0, temY = 0, i, temJob = 0, pTx=-1, pTy=-1;
	
	struct peca *peca = ((struct peca *) g);
	
	vez = (peca->referencia)->vez;
	pX = peca->x;
	pY = peca->y;
	pT = (peca->referencia)->tipo[pX][pY];
	
	  for(i = 0; i < 3; i++)
	    if(pX == (peca->referencia)->xJob[i]){
	      temX = 1;
	      break;
	    }
	  for(i = 0; i < 3; i++)
	    if(pY == (peca->referencia)->yJob[i]){
	      temY = 1;
	      break;
	    }
	    
	if((peca->referencia)->quant_pecasVerdes == 0 || (peca->referencia)->quant_pecasAmarelas == 0){
	  jogadorStatusbar(NULL, NULL);
	  return;
	}
	
	if((peca->referencia)->job == 1 && (temX == 0 || temY == 0) && pecaLigada == 0){
	  if(pT != 50){
	    PopUp("Existe jogada obrigatória a ser feita\n", "#cc0000", 0);
	    return;
	  }
	}
	
	if((pT == vez) || (pT == vez*10) || (pT == 50 && pecaLigada == 1)){
	  image = GTK_IMAGE(peca->imagem);
	  
	if(imageAux == NULL)
	  imageAux = GTK_IMAGE(peca->imagem); 
	  
	if(clickouUmaVez == 0){
	  pXAux = pX;
	  pYAux = pY;
	  pTAux = (peca->referencia)->tipo[pX][pY];
	  clickouUmaVez = 1;
	}
	  switch(pT){
	    
	    case 1: // Caso o click venha de uma peça verde
	      if(pecaLigada == 0 && vez == 1){
		gtk_image_set_from_file(image , PECA_VERDE_CLICKADA);
		pecaLigada = 1;
		(peca->referencia)->tipo[pXAux][pYAux] = 1;
		pXAux = pX;
		pYAux = pY;
		imageAux = image;
		
		/* Verifica se à jogada possível para a direita */
		if((peca->referencia)->tipo[pX+1][pY+1] == 0 && pX < 7 && pY < 7 && (peca->referencia)->job == 0){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY+1])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		  (peca->referencia)->tipo[pX+1][pY+1] = 50;
		}
		/* Verifica se é possível comer para a direita */
		else if((((peca->referencia)->tipo[pX+1][pY+1] == 2 || (peca->referencia)->tipo[pX+1][pY+1] == 20) && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX < 6 && pY < 6)){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+2][pY+2])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		    (peca->referencia)->tipo[pX+2][pY+2] = 50;
		}
		/* Verifica se à jogada possível para a esquerda */
		if((peca->referencia)->tipo[pX+1][pY-1] == 0 && pX < 7 && pY > 0  && (peca->referencia)->job == 0){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY-1])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		  (peca->referencia)->tipo[pX+1][pY-1] = 50;
		}
		/* Verifica se é possível comer para a esquerda */
		else if((((peca->referencia)->tipo[pX+1][pY-1] == 2 || (peca->referencia)->tipo[pX+1][pY-1] == 20) && (peca->referencia)->tipo[pX+2][pY-2] == 0)  && (pX < 6 && pY > 1)){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+2][pY-2])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		    (peca->referencia)->tipo[pX+2][pY-2] = 50;
		}
		  
	      } else if(vez == 1 && pTAux == 1) { //Se a peça estiver ligada e receber um click, ela sera desligada.
		if((pXAux == pX) && (pYAux == pY)){
		  gtk_image_set_from_file(image , PECA_VERDE);
		  pecaLigada = 0;
		  (peca->referencia)->tipo[pXAux][pYAux] = 1;
		  imageAux = NULL;
		  clickouUmaVez = 0;
		  limpaPJogadas(g); // Chama função para limpar as imagens de jogadas possíveis
		}
	      }
	    break; // Caso o click venha de uma peça verde
	    
	    case 2: // Caso o click venha de uma peça amarela
	      if(pecaLigada == 0 && vez == 2){
		gtk_image_set_from_file(image , PECA_AMARELA_CLICKADA);
		pecaLigada = 1;
		(peca->referencia)->tipo[pXAux][pYAux] = 2;
		pXAux = pX;
		pYAux = pY;
		imageAux = image;
		
		/* Verifica se à jogada possível para a direita */
		if((peca->referencia)->tipo[pX-1][pY+1] == 0 && pX > 0 && pY < 7 && (peca->referencia)->job == 0){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY+1])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		  (peca->referencia)->tipo[pX-1][pY+1] = 50;
		}
		/* Verifica se é possível comer para a direita */
		else if((((peca->referencia)->tipo[pX-1][pY+1] == 1 || (peca->referencia)->tipo[pX-1][pY+1] == 10) && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX > 1 && pY < 6)){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-2][pY+2])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		    (peca->referencia)->tipo[pX-2][pY+2] = 50;
		}
		
		/* Verifica se à jogada possível para a esquerda */
		if((peca->referencia)->tipo[pX-1][pY-1] == 0 && pX > 0 && pY > 0 && (peca->referencia)->job == 0){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY-1])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		  (peca->referencia)->tipo[pX-1][pY-1] = 50;
		}
		/* Verifica se é possível comer para a esquerda */
		else if((((peca->referencia)->tipo[pX-1][pY-1] == 1 || (peca->referencia)->tipo[pX-1][pY-1] == 10) && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX > 1 && pY > 1)){
		  imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-2][pY-2])->imagem);
		  gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
		    (peca->referencia)->tipo[pX-2][pY-2] = 50;
		}
		  
	      } else if(vez == 2 && pTAux == 2) {
		if((pXAux == pX) && (pYAux == pY)){ // Se a peça ligada e receber um click ela será desligada
		  gtk_image_set_from_file(image , PECA_AMARELA);
		  pecaLigada = 0;
		  (peca->referencia)->tipo[pXAux][pYAux] = 2;
		  imageAux = NULL;
		  clickouUmaVez = 0;
		  limpaPJogadas(g); // Chama função para limpar as imagens de jogadas possíveis
		}
	      }
	    break; // Caso o click venha de uma peça amarela
	    
	    case 10: // Caso o click venha de uma dama verde
	      if(pecaLigada == 0 && vez == 1){
		gtk_image_set_from_file(image , DAMA_VERDE_CLICKADA);
		pecaLigada = 1;
		(peca->referencia)->tipo[pXAux][pYAux] = 10;
		pXAux = pX;
		pYAux = pY;
		imageAux = image;
		possivelJogadaDama(g, pX, pY, 10);
		
	      } else if(vez == 1 && pTAux == 10) { //Se a peça estiver ligada e receber um click, ela sera desligada.
		if((pXAux == pX) && (pYAux == pY)){
		  gtk_image_set_from_file(image , DAMA_VERDE);
		  pecaLigada = 0;
		  (peca->referencia)->tipo[pXAux][pYAux] = 10;
		  imageAux = NULL;
		  clickouUmaVez = 0;
		  limpaPJogadas(g); // Chama função para limpar as imagens de jogadas possíveis
		}
	      }
	    break; // Caso o click venha de uma dama verde
	    
	    case 20: // Caso o click venha de uma dama amarela
	      if(pecaLigada == 0 && vez == 2){
		gtk_image_set_from_file(image , DAMA_AMARELA_CLICKADA);
		pecaLigada = 1;
		(peca->referencia)->tipo[pXAux][pYAux] = 20;
		pXAux = pX;
		pYAux = pY;
		imageAux = image;
		possivelJogadaDama(g, pX, pY, 20);
		  
	      } else if(vez == 2 && pTAux == 20) { //Se a peça estiver ligada e receber um click, ela sera desligada.
		if((pXAux == pX) && (pYAux == pY)){
		  gtk_image_set_from_file(image , DAMA_AMARELA);
		  pecaLigada = 0;
		  (peca->referencia)->tipo[pXAux][pYAux] = 20;
		  imageAux = NULL;
		  clickouUmaVez = 0;
		  limpaPJogadas(g); // Chama função para limpar as imagens de jogadas possíveis
		}
	      }
	    break; // Caso o click venha de uma dama amarela
	    
	    case 50: // Caso o click venha de uma jogada possível. Aqui onde temos todas as regras do jogo. WARNING
		  if(pTAux == 1 && pX - pXAux == 1){  //Simples troca de verde/preto
		      vez = 2;
		      gtk_image_set_from_file(imageAux, BOX_PRETO);
		      gtk_image_set_from_file(image, PECA_VERDE);
		      (peca->referencia)->tipo[pX][pY] = 1;
		      (peca->referencia)->tipo[pXAux][pYAux] = 0;
		      imageAux = NULL;
		      pecaLigada = 0;
		      clickouUmaVez = 0;
		      limpaPJogadas(g);
		  } else if(pTAux == 2 && pX - pXAux == -1){ //Simples troca de amarelo/preto
		      vez = 1;
		      gtk_image_set_from_file(imageAux, BOX_PRETO);
		      gtk_image_set_from_file(image, PECA_AMARELA);
		      (peca->referencia)->tipo[pX][pY] = 2;
		      (peca->referencia)->tipo[pXAux][pYAux] = 0;
		      imageAux = NULL;
		      pecaLigada = 0;
		      clickouUmaVez = 0;
		      limpaPJogadas(g);
		  } else if(pTAux == 1 && pX - pXAux == 2) { //Peça Verde come a Amarela ou Dama Amarela.
		      vez = 2;
		      if((((peca->referencia)->tipo[pX+1][pY-1] == 2 || (peca->referencia)->tipo[pX+1][pY-1] == 20) && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2))
			vez = 1;
		      else if((((peca->referencia)->tipo[pX+1][pY+1] == 2 || (peca->referencia)->tipo[pX+1][pY+1] == 20) && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5))
			vez = 1;
		    if(((pX == pXAux+2) && (pY == pYAux-2) && ((peca->referencia)->tipo[pX-1][pY+1]) == 2 || ((peca->referencia)->tipo[pX-1][pY+1]) == 20)){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY+1])->imagem);
		      (peca->referencia)->tipo[pX-1][pY+1] = 0;
		    } else {
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY-1])->imagem);
		      (peca->referencia)->tipo[pX-1][pY-1] = 0;
		    }
		    
		    gtk_image_set_from_file(image, PECA_VERDE);
		    gtk_image_set_from_file(imageAux, BOX_PRETO);
		    gtk_image_set_from_file(imageApaga, BOX_PRETO);
		    
		    (peca->referencia)->tipo[pXAux][pYAux] = 0;
		    (peca->referencia)->tipo[pX][pY] = 1;
		    (peca->referencia)->quant_pecasAmarelas--;
		    pXAux = pX;
		    pYAux = pY;
		    pecaLigada = 0;
		    clickouUmaVez = 0;
		    limpaPJogadas(g);
		  } else if(pTAux == 2 && pX - pXAux == -2 ) { // Peças Amarelas come a Verde ou Dama Verde
		    vez = 1;
		      if((((peca->referencia)->tipo[pX-1][pY-1] == 1 || (peca->referencia)->tipo[pX-1][pY-1] == 10) && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2))
			vez = 2;
		      else if((((peca->referencia)->tipo[pX-1][pY+1] == 1 || (peca->referencia)->tipo[pX-1][pY+1] == 10) && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2  && pY <= 5))
			vez = 2;
		    
		      if(((pX == pXAux-2) && (pY == pYAux+2) && ((peca->referencia)->tipo[pX+1][pY-1]) == 1 || ((peca->referencia)->tipo[pX+1][pY-1]) == 10)){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY-1])->imagem);
		      gtk_image_set_from_file(image, PECA_AMARELA);
		      (peca->referencia)->tipo[pX+1][pY-1] = 0;
		      } else {
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY+1])->imagem);
		      gtk_image_set_from_file(image, PECA_AMARELA);
		      (peca->referencia)->tipo[pX+1][pY+1] = 0;
		    }
		    
		    gtk_image_set_from_file(imageAux, BOX_PRETO);
		    gtk_image_set_from_file(imageApaga, BOX_PRETO);
		    
		    (peca->referencia)->tipo[pXAux][pYAux] = 0;
		    (peca->referencia)->tipo[pX][pY] = 2;
		    (peca->referencia)->quant_pecasVerdes--;
		    pXAux = pX;
		    pYAux = pY;
		    pecaLigada = 0;
		    clickouUmaVez = 0;
		    limpaPJogadas(g);
		  } else if(pTAux == 10 && (pX - pXAux == 1 || pX - pXAux == -1)){  //Simples troca de  dama verde / preto
		      vez = 2;
		      gtk_image_set_from_file(imageAux, BOX_PRETO);
		      gtk_image_set_from_file(image, DAMA_VERDE);
		      (peca->referencia)->tipo[pX][pY] = 10;
		      (peca->referencia)->tipo[pXAux][pYAux] = 0;
		      imageAux = NULL;
		      pecaLigada = 0;
		      clickouUmaVez = 0;
		      limpaPJogadas(g);
		  } else if(pTAux == 20 && (pX - pXAux == 1 || pX - pXAux == -1)){  //Simples troca de  dama amarela / preto
		      vez = 1;
		      gtk_image_set_from_file(imageAux, BOX_PRETO);
		      gtk_image_set_from_file(image, DAMA_AMARELA);
		      (peca->referencia)->tipo[pX][pY] = 20;
		      (peca->referencia)->tipo[pXAux][pYAux] = 0;
		      imageAux = NULL;
		      pecaLigada = 0;
		      clickouUmaVez = 0;
		      limpaPJogadas(g);
		  } else if(pTAux == 20 && (pX - pXAux == 2 || pX - pXAux == -2)) { //Dama amarela come peça ou dama
		    if(pXAux - pX == 2 && pYAux-pY == 2){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY+1])->imagem);
		      (peca->referencia)->tipo[pX+1][pY+1] = 0;
		    }
		    else if(pXAux - pX == 2 && pYAux-pY == -2){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY-1])->imagem);
		      (peca->referencia)->tipo[pX+1][pY-1] = 0;
		    }
		    else if(pXAux - pX == -2 && pYAux-pY == 2){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY+1])->imagem);
		      (peca->referencia)->tipo[pX-1][pY+1] = 0;
		    } else if(pXAux - pX == -2 && pYAux-pY == -2) {
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY-1])->imagem);
		      (peca->referencia)->tipo[pX-1][pY-1] = 0;
		    }
		    
		    gtk_image_set_from_file(image, DAMA_AMARELA);
		    gtk_image_set_from_file(imageAux, BOX_PRETO);
		    gtk_image_set_from_file(imageApaga, BOX_PRETO);
		    
		    vez = 1;
		      if((((peca->referencia)->tipo[pX-1][pY-1] == 1 || (peca->referencia)->tipo[pX-1][pY-1] == 10) && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2))
			vez = 2;
		      else if((((peca->referencia)->tipo[pX-1][pY+1] == 1 || (peca->referencia)->tipo[pX-1][pY+1] == 10) && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2 && pY <= 5 ))
			vez = 2;
		      if((((peca->referencia)->tipo[pX+1][pY-1] == 1 || (peca->referencia)->tipo[pX+1][pY-1] == 10) && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2))
			vez = 2;
		      else if((((peca->referencia)->tipo[pX+1][pY+1] == 1 || (peca->referencia)->tipo[pX+1][pY+1] == 10) && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5 ))
			vez = 2;
		    (peca->referencia)->tipo[pXAux][pYAux] = 0;
		    (peca->referencia)->tipo[pX][pY] = 20;
		    (peca->referencia)->quant_pecasVerdes--;
		    pXAux = pX;
		    pYAux = pY;
		    pecaLigada = 0;
		    clickouUmaVez = 0;
		    limpaPJogadas(g);
		  }
		  else if(pTAux == 10 && (pX - pXAux == 2 || pX - pXAux == -2)) { //Dama verde come peça ou dama
		    if(pXAux - pX == 2 && pYAux-pY == 2){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY+1])->imagem);
		      (peca->referencia)->tipo[pX+1][pY+1] = 0;
		    }
		    else if(pXAux - pX == 2 && pYAux-pY == -2){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY-1])->imagem);
		      (peca->referencia)->tipo[pX+1][pY-1] = 0;
		    }
		    else if(pXAux - pX == -2 && pYAux-pY == 2){
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY+1])->imagem);
		      (peca->referencia)->tipo[pX-1][pY+1] = 0;
		    } else if(pXAux - pX == -2 && pYAux-pY == -2) {
		      imageApaga = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY-1])->imagem);
		      (peca->referencia)->tipo[pX-1][pY-1] = 0;
		    }
		    
		    gtk_image_set_from_file(image, DAMA_VERDE);
		    gtk_image_set_from_file(imageAux, BOX_PRETO);
		    gtk_image_set_from_file(imageApaga, BOX_PRETO);
		    
		    vez = 2;
		      if((((peca->referencia)->tipo[pX+1][pY-1] == 2 || (peca->referencia)->tipo[pX+1][pY-1] == 20) && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2))
			vez = 1;
		      else if((((peca->referencia)->tipo[pX+1][pY+1] == 2 || (peca->referencia)->tipo[pX+1][pY+1] == 20) && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5 ))
			vez = 1;
		      if((((peca->referencia)->tipo[pX-1][pY-1] == 2 || (peca->referencia)->tipo[pX-1][pY-1] == 20) && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2))
			vez = 1;
		      else if((((peca->referencia)->tipo[pX-1][pY+1] == 2 || (peca->referencia)->tipo[pX-1][pY+1] == 20) && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2 && pY <= 5 ))
			vez = 1;
		    (peca->referencia)->tipo[pXAux][pYAux] = 0;
		    (peca->referencia)->tipo[pX][pY] = 10;
		    (peca->referencia)->quant_pecasAmarelas--;
		    pXAux = pX;
		    pYAux = pY;
		    pecaLigada = 0;
		    clickouUmaVez = 0;
		    limpaPJogadas(g);
		  }
		  else {
		  pecaLigada = 1;
		  clickouUmaVez = 0;
		  }
	    break;
	    }
	  }
	  else if(pT == 0 && pecaLigada == 0){
	    return;
	  }
	  else if(pT != vez && pT != 0) {
	    if(vez == 1)
	      PopUp((peca->referencia)->jogador1, "#cc0000", 1);
	    else
	       PopUp((peca->referencia)->jogador2, "#cc0000", 1);
	    return ;
	  }
	  (peca->referencia)->vez = vez;
	  testaJogo(g);
	  
	  jogadorStatusbar(NULL, g);
}

/* Verifica Jogada de Dama ************************************************************************************************/
void possivelJogadaDama(gpointer g, int pX, int pY, int dama)
{
  struct peca *peca = ((struct peca *) g);
  GtkImage *imageJogada;
  int pecaOposta, damaOposta;
  
  if(dama == 10){
    pecaOposta = 2;
    damaOposta = 20;
  }
  else {
    pecaOposta = 1;
    damaOposta = 10;
  }
  
  /* Andar para baixo */
  /* Verifica se à jogada possível para a direita */
  if((peca->referencia)->tipo[pX+1][pY+1] == 0 && pX < 7 && pY < 7 && (peca->referencia)->job == 0){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY+1])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX+1][pY+1] = 50;
  }
  
  /* Verifica se é possível comer para a direita */
  else if((((peca->referencia)->tipo[pX+1][pY+1] == pecaOposta || (peca->referencia)->tipo[pX+1][pY+1] == damaOposta) && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX < 6 && pY < 6)){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+2][pY+2])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX+2][pY+2] = 50;
  }
  
  /* Verifica se à jogada possível para a esquerda */
  if((peca->referencia)->tipo[pX+1][pY-1] == 0 && pX < 7 && pY > 0  && (peca->referencia)->job == 0){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+1][pY-1])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX+1][pY-1] = 50;
  }
  
  /* Verifica se é possível comer para a esquerda */
  else if((((peca->referencia)->tipo[pX+1][pY-1] == pecaOposta || (peca->referencia)->tipo[pX+1][pY-1] == damaOposta) && (peca->referencia)->tipo[pX+2][pY-2] == 0)  && (pX < 6 && pY > 1)){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX+2][pY-2])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX+2][pY-2] = 50;
  }
  /* Fim do Andar para baixo */
		
  /* Andar para cima */
  /* Verifica se à jogada possível para a direita */
  if((peca->referencia)->tipo[pX-1][pY+1] == 0 && pX > 0 && pY < 7  && (peca->referencia)->job == 0){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY+1])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX-1][pY+1] = 50;
  }
  
  /* Verifica se é possível comer para a direita */
  else if((((peca->referencia)->tipo[pX-1][pY+1] == pecaOposta || (peca->referencia)->tipo[pX-1][pY+1] == damaOposta) && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX > 1 && pY < 6)){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-2][pY+2])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX-2][pY+2] = 50;
  }
		
  /* Verifica se à jogada possível para a esquerda */
  if((peca->referencia)->tipo[pX-1][pY-1] == 0 && pX > 0 && pY > 0  && (peca->referencia)->job == 0){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-1][pY-1])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX-1][pY-1] = 50;
  }
  
  /* Verifica se é possível comer para a esquerda */
  else if((((peca->referencia)->tipo[pX-1][pY-1] == pecaOposta || (peca->referencia)->tipo[pX-1][pY-1] == damaOposta) && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX > 1 && pY > 1)){
    imageJogada = GTK_IMAGE(((peca->referencia)->referenciadapeca[pX-2][pY-2])->imagem);
    gtk_image_set_from_file(imageJogada, BOX_POSSIVEL_JOGADA);
    (peca->referencia)->tipo[pX-2][pY-2] = 50;
  }
  /* Fim do Andar para cima */
}

/* PopUp ******************************************************************************************************************/
void PopUp(gchar *message, gchar *cor, gint tipomsg)
{

	char msg[100];
	GtkWidget *dialog, *label;
   
	dialog = gtk_dialog_new_with_buttons ("Damas Player",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_NONE,
                                         NULL);
	gtk_window_set_icon(GTK_WINDOW(dialog), criaIcone(ICONE));
	gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
	gtk_widget_set_usize(dialog,330,100);                                         
	label = gtk_label_new (message);
	if(tipomsg == 0)
	  sprintf(msg,"<span foreground=\"%s\">%s</span>",cor, message);
	else if(tipomsg == 2)
	  sprintf(msg,"<span foreground=\"%s\">Fim de Jogo:\nO vecendor foi <b>%s</b></span>",cor, message);
	else
	  sprintf(msg,"<span foreground=\"%s\">É vez de %s jogar!</span>",cor, message);
	gtk_label_set_markup (GTK_LABEL (label),msg);
	gtk_label_set_justify(GTK_LABEL (label),GTK_JUSTIFY_CENTER);
	g_signal_connect_swapped (dialog,"response",G_CALLBACK (gtk_widget_destroy),dialog);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),label);
	gtk_widget_show_all (dialog);
}

/* Função para Novo Jogo **************************************************************************************************/
void NovoJogoQ(GtkWidget *w, gpointer g){
	struct table *tabela = ((struct table *) g);
	if(tabela->quant_pecasVerdes == 0 || tabela->quant_pecasAmarelas == 0)
	  NovoJogoF(NULL, g);
	else {
	  GtkWidget *dialog;
   
	  dialog = gtk_message_dialog_new(NULL, 
					  GTK_DIALOG_DESTROY_WITH_PARENT, 
					  GTK_MESSAGE_QUESTION, 
					  GTK_BUTTONS_YES_NO,
					  "Você deseja iniciar um novo jogo?");
	  gtk_window_set_title(GTK_WINDOW(dialog), "Damas Player - Novo Jogo");
	  gint result = gtk_dialog_run (GTK_DIALOG (dialog));
	  gtk_widget_destroy(dialog);
	  
	  if(result == -8)
	    NovoJogoF(NULL, g);
	}
}

/* Função para Novo Jogo **************************************************************************************************/
void NovoJogoF(GtkWidget *w, gpointer g){
	struct table *tabela = ((struct table *) g);
	
	int i, j, casas=0, casasVerdes, pecasV=0, pecasA=0;
	GtkImage *image;

	
	tabela->quant_pecasAmarelas = 12;
	tabela->quant_pecasVerdes = 12;
	for(i=0; i < 8; i++){
	  for(j=0; j < 8; j++){
 	    if((i-j) % 2 != 0){
 	      if(pecasV < 12){ 
		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
		gtk_image_set_from_file(image, PECA_VERDE);
		pecasV++;
		tabela->tipo[i][j] = 1;
 	      } else if ((casas >= 40) && (pecasA < 12)){
 		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
 		gtk_image_set_from_file(image, PECA_AMARELA);
 		pecasA++;
 		tabela->tipo[i][j] = 2;
 	      } else {
 		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
 		gtk_image_set_from_file(image, BOX_PRETO);
 		pecasV++;
 		tabela->tipo[i][j] = 0;
 	      }
 	    }
 	    casas++;
	  } 
     }
     gravaNomesJogadores(g);
}

/* Função 'Pega' Nome *****************************************************************************************************/
void gravaNomesJogadores(gpointer g){
  struct table *tabela = ((struct table *) g);
  
  char *entry1Text = "Peças Verdes", *entry2Text = "Peças Amarelas";
  const gchar *jogador2, *jogador1;
  gint result;
  GtkWidget *dialog, *label, *tablew, *label1, *label2, *entry1, *entry2;
  
  dialog = gtk_dialog_new_with_buttons ("Damas Player - Novo Jogo",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_NONE,
                                         NULL);
	gtk_window_set_icon(GTK_WINDOW(dialog), criaIcone(ICONE));
	gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
	gtk_widget_set_usize(dialog,270,130);
	tablew = gtk_table_new(3, 2, FALSE);
 	gtk_container_add(GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), tablew);
	label1 = gtk_label_new("Jogador 1:");
	label2 = gtk_label_new("Jogador 2:");
	gtk_table_attach(GTK_TABLE(tablew), label1, 0, 1, 0, 1,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(tablew), label2, 0, 1, 1, 2, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	entry1 =  gtk_entry_new_with_max_length(19);
	entry2 =  gtk_entry_new_with_max_length(19);
 	gtk_entry_set_text(GTK_ENTRY(entry1), entry1Text);
	gtk_entry_set_text(GTK_ENTRY(entry2), entry2Text);
	gtk_table_attach(GTK_TABLE(tablew), entry1, 1, 2, 0, 1, GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_table_attach(GTK_TABLE(tablew), entry2, 1, 2, 1, 2,GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 5, 5);
	gtk_widget_show(tablew);
	gtk_widget_show(label1);
	gtk_widget_show(label2);
	gtk_widget_show(entry1);
	gtk_widget_show(entry2);
	
    result = gtk_dialog_run (GTK_DIALOG (dialog));
    if(result == -1){
     strcpy((char *)tabela->jogador2,(char *)gtk_entry_get_text(GTK_ENTRY(entry2)));
     strcpy((char *)tabela->jogador1,(char *)gtk_entry_get_text(GTK_ENTRY(entry1)));
    }
    jogadorStatusbar(g, NULL);
    gtk_widget_destroy(dialog);
  
}
/* Funçao Regras **********************************************************************************************************/
void regrasF(GtkWidget *w, gpointer p){
  
	char msg[950];
	GtkWidget *dialog, *label, *img;
 	gchar *message = "\t\t\t\t\t  <b>Regras do Jogo:</b>\n\n<b>-</b> O jogo é praticado em um tabuleiro de 64 casas, claras e escuras;\n\n<b>-</b> O objetivo do jogo é imobilizar ou capturar todas as peças do\nadversário;\n\n<b>-</b> Esse jogo é praticado entre dois parceiros, com 12 pedras verdes de um\n lado e com 12 pedras amarelas de outro lado;\n\n<b>-</b> A peça anda uma casa de cada vez para frente, e ganha peça do\nadversário andando para frente, quando a peça chega a oitava casa é\n  promovida para Dama;\n\n<b>-</b> A dama pode andar tanto para frente quanto para trás, mas só uma\ncasa por vez;\n\n<b>-</b> A peça captura a dama e a dama captura a peça;\n\n<b>-</b> Pode-se capturar mais de uma peça por vez, desde que seja possível\nexecutar tal jogada;\n\n<b>-</b> Na execução do lance de captura, é permitido passar mais de uma vez\npela mesma casa vazia, porém, não é permitido capturar duas vezes a\n mesma peça.";
	dialog = gtk_dialog_new_with_buttons ("Damas Player - Regras do Jogo",
                                         NULL,
                                         GTK_DIALOG_MODAL,
                                         GTK_STOCK_OK,
                                         GTK_RESPONSE_NONE,
                                         NULL);
	img = gtk_image_new_from_file(SCREEN_GAME);
	gtk_window_set_icon(GTK_WINDOW(dialog), criaIcone(ICONE));
	gtk_window_set_resizable(GTK_WINDOW(dialog),FALSE);
	gtk_widget_set_usize(dialog,500,500);                                         
	label = gtk_label_new (message);
	  sprintf(msg,"<span foreground=\"#000000\">%s</span>", message);
	gtk_label_set_markup (GTK_LABEL (label),msg);
	gtk_label_set_justify(GTK_LABEL (label),GTK_JUSTIFY_LEFT);
	g_signal_connect_swapped (dialog,"response",G_CALLBACK (gtk_widget_destroy),dialog);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox),label);
	gtk_widget_show_all (dialog);
	
}

/* Função para Verificar se tem nova Dama *********************************************************************************/
void testaJogo(gpointer *g){
  int x, y, i=0, pX=0, pY=0;
  GtkImage *image;
  struct peca *peca = ((struct peca *) g);
  
    for(x=0; x<8; x++)
      if((peca->referencia)->tipo[0][x] == 2){
	(peca->referencia)->tipo[0][x] = 20;
	image = GTK_IMAGE(((peca->referencia)->referenciadapeca[0][x])->imagem);
	gtk_image_set_from_file(image, DAMA_AMARELA);
	break;
      }
      
      for(x=0; x<8; x++)
      if((peca->referencia)->tipo[7][x] == 1){
	(peca->referencia)->tipo[7][x] = 10;
	image = GTK_IMAGE(((peca->referencia)->referenciadapeca[7][x])->imagem);
	gtk_image_set_from_file(image, DAMA_VERDE);
	break;
      }
      
       if((peca->referencia)->quant_pecasAmarelas == 0)
	 PopUp((peca->referencia)->jogador1, "#000000", 2);
       else if((peca->referencia)->quant_pecasVerdes == 0)
	 PopUp((peca->referencia)->jogador2, "#000000", 2);
      
      for(y=0; y<3; y++){
	(peca->referencia)->xJob[y] = -1;
	(peca->referencia)->yJob[y] = -1;
      }
      (peca->referencia)->job = 0;
      for(x=0; x<8; x++){
	for(y=0; y<8; y++){
	  pX = x;
	  pY = y;
	  if((((peca->referencia)->tipo[pX][pY] == 1 || (peca->referencia)->tipo[pX][pY] == 10) && (peca->referencia)->tipo[pX+1][pY-1] == 2 && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2) && (peca->referencia)->vez == 1){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if((((peca->referencia)->tipo[pX][pY] == 1 || (peca->referencia)->tipo[pX][pY] == 10) && (peca->referencia)->tipo[pX+1][pY+1] == 2 && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5 )  && (peca->referencia)->vez == 1) {
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if((((peca->referencia)->tipo[pX][pY] == 1 || (peca->referencia)->tipo[pX][pY] == 10) && (peca->referencia)->tipo[pX+1][pY-1] == 20 && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2)  && (peca->referencia)->vez == 1) {
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if((((peca->referencia)->tipo[pX][pY] == 1 || (peca->referencia)->tipo[pX][pY] == 10) && (peca->referencia)->tipo[pX+1][pY+1] == 20 && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5 )  && (peca->referencia)->vez == 1){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
 	  }
 	  if(((peca->referencia)->tipo[pX][pY] == 10 && (peca->referencia)->tipo[pX-1][pY-1] == 2 && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2)  && (peca->referencia)->vez == 1) {
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(((peca->referencia)->tipo[pX][pY] == 10 && (peca->referencia)->tipo[pX-1][pY+1] == 2 && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2 && pY <= 5 )  && (peca->referencia)->vez == 1) {
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(((peca->referencia)->tipo[pX][pY] == 10 && (peca->referencia)->tipo[pX-1][pY-1] == 20 && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2)  && (peca->referencia)->vez == 1) {
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  }else if(((peca->referencia)->tipo[pX][pY] == 10 && (peca->referencia)->tipo[pX-1][pY+1] == 20 && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2 && pY <= 5 )  && (peca->referencia)->vez == 1) {
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  }
	  // Amarelas
	   if((((peca->referencia)->tipo[pX][pY] == 2 || (peca->referencia)->tipo[pX][pY] == 20) && (peca->referencia)->tipo[pX-1][pY-1] == 1 && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if((((peca->referencia)->tipo[pX][pY] == 2 || (peca->referencia)->tipo[pX][pY] == 20) && (peca->referencia)->tipo[pX-1][pY+1] == 1 && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2 && pY <= 5 ) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(( ((peca->referencia)->tipo[pX][pY] == 2 || (peca->referencia)->tipo[pX][pY] == 20) && (peca->referencia)->tipo[pX-1][pY-1] == 10 && (peca->referencia)->tipo[pX-2][pY-2] == 0) && (pX >= 2 && pY >= 2) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(( ((peca->referencia)->tipo[pX][pY] == 2 || (peca->referencia)->tipo[pX][pY] == 20) && (peca->referencia)->tipo[pX-1][pY+1] == 10 && (peca->referencia)->tipo[pX-2][pY+2] == 0) && (pX >= 2 && pY <= 5 ) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  }
	    if(((peca->referencia)->tipo[pX][pY] == 20 && (peca->referencia)->tipo[pX+1][pY-1] == 1 && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(((peca->referencia)->tipo[pX][pY] == 20 && (peca->referencia)->tipo[pX+1][pY+1] == 1 && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5 ) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(((peca->referencia)->tipo[pX][pY] == 20 && (peca->referencia)->tipo[pX+1][pY-1] == 10 && (peca->referencia)->tipo[pX+2][pY-2] == 0) && (pX <= 5 && pY >= 2) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  } else if(((peca->referencia)->tipo[pX][pY] == 20 && (peca->referencia)->tipo[pX+1][pY+1] == 10 && (peca->referencia)->tipo[pX+2][pY+2] == 0) && (pX <= 5 && pY <= 5 ) && (peca->referencia)->vez == 2){
	    (peca->referencia)->xJob[i] = x;
	    (peca->referencia)->yJob[i] = y;
	    (peca->referencia)->job = 1;
	    i++;
	  }
	 if(i == 2)
	   return;
	}
      }
     
}

/* Função Limpar as imagens de possíveis jogadas **************************************************************************/
void limpaPJogadas(gpointer *g){
  int x, z;
  GtkImage *image;
  
  struct peca *peca = ((struct peca *) g);
  
    for(x=0; x<8; x++)
      for(z=0; z<8; z++)
      if((peca->referencia)->tipo[x][z] == 50){
	(peca->referencia)->tipo[x][z] = 0;
	image = GTK_IMAGE(((peca->referencia)->referenciadapeca[x][z])->imagem);
	gtk_image_set_from_file(image, BOX_PRETO);
      }
}

/* Função Salva Jogo **************************************************************************************************/
void salvaJogo(GtkWidget *w, gpointer g){
	
	FILE *pa;
	char *nome = "save/jogoSalvo.dms";
	char jogador[20];

	 if((pa = fopen(nome, "w+")) == NULL){
	  PopUp("Não foi possível salvar o jogo!\n", "#cc0000", 0);
	  return;
	 }
	struct table *tabela = ((struct table *) g);
	int i, j;

	for(i=0; i < 8; i++)
	  for(j=0; j < 8; j++)  
	    fprintf(pa, "%d ", tabela->tipo[i][j]);
	
	fprintf(pa, " %d %d %d %d", tabela->vez, tabela->quant_pecasVerdes, tabela->quant_pecasAmarelas, tabela->job);
	
	for(i=0; i<19; i++)
	  if(tabela->jogador1[i] == ' ')
	    jogador[i] = '+';
	  else
	    jogador[i] = tabela->jogador1[i];
	
	fprintf(pa, " %s", jogador);
	
	for(i=0; i<19; i++)
	  if(tabela->jogador2[i] == ' ')
	    jogador[i] = '+';
	  else
	    jogador[i] = tabela->jogador2[i];
	
	fprintf(pa, " %s", jogador);
	fprintf(pa, " %d %d %d %d %d %d", tabela->xJob[0], tabela->xJob[1], tabela->xJob[2], tabela->yJob[0], tabela->yJob[1], tabela->yJob[2]);
	PopUp("Jogo salvo!\n", "#cc0000", 0);
	fclose(pa);
}

/* Função Carregar Jogo ***************************************************************************************************/
void carregaJogoF(GtkWidget *w, gpointer g){
	
	FILE *pa;
	char *nome = "save/jogoSalvo.dms";
	char jogador1[20], jogador2[20];
	
	 if((pa = fopen(nome, "r")) == NULL){
	  PopUp("Não foi possível carregar o jogo!\n", "#cc0000", 0);
	  return;
	 }
	struct table *tabela = ((struct table *) g);
	int i, j;
	GtkImage *image;

	for(i=0; i < 8; i++)
	  for(j=0; j < 8; j++)
	    fscanf(pa, "%d", &tabela->tipo[i][j]);
	
	fscanf(pa, "%d%d%d%d%s%s", &tabela->vez, &tabela->quant_pecasVerdes, &tabela->quant_pecasAmarelas, &tabela->job, jogador1, jogador2);
	fscanf(pa, "%d%d%d%d%d%d", &tabela->xJob[0], &tabela->xJob[1], &tabela->xJob[2], &tabela->yJob[0], &tabela->yJob[1], &tabela->yJob[2]);
	
	for(i=0; i<19; i++){
	  tabela->jogador1[i] = ' ';
	  tabela->jogador2[i] = ' ';
	}
	tabela->jogador1[19] = '\0';
	tabela->jogador2[19] = '\0';
	
	for(i=0; jogador1[i] != '\0'; i++)
	  if(jogador1[i] == '+')
	    tabela->jogador1[i] = ' ';
	  else
	    tabela->jogador1[i] = jogador1[i];

	
	for(i=0; jogador2[i] != '\0'; i++)
	  if(jogador2[i] == '+')
	    tabela->jogador2[i] = ' ';
	  else
	    tabela->jogador2[i] = jogador2[i];
	
	fclose(pa);
	
	for(i=0; i < 8; i++){
	  for(j=0; j < 8; j++){
 	    if(tabela->tipo[i][j] == 1){ 
		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
		gtk_image_set_from_file(image, PECA_VERDE);
 	      } else if (tabela->tipo[i][j] == 2){
 		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
 		gtk_image_set_from_file(image, PECA_AMARELA);
 	      } else if(tabela->tipo[i][j] == 0) {
 		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
 		gtk_image_set_from_file(image, BOX_PRETO);
 	      } else if(tabela->tipo[i][j] == 10) {
 		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
 		gtk_image_set_from_file(image, DAMA_VERDE);
	      } else if(tabela->tipo[i][j] == 20) {
 		image = GTK_IMAGE((tabela->referenciadapeca[i][j])->imagem);
 		gtk_image_set_from_file(image, DAMA_AMARELA);
 	      }
 	    }
	  }
      PopUp("Jogo carregado!\n", "#cc0000", 0);
      jogadorStatusbar(g, NULL);
     }
     
void jogadorStatusbar(gpointer g, gpointer k){
  gchar *str, *jogador;
  
  if(g != NULL){
    struct table *tabela = ((struct table *) g);
  
  if(tabela->vez == 1)
    jogador = tabela->jogador1;
  else
    jogador = tabela->jogador2;
  
  str = g_strdup_printf("Jogador da Vez: %s", jogador);
  gtk_statusbar_push(GTK_STATUSBAR(tabela->statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR(tabela->statusbar), str), str);
  }
  
  if(k != NULL){
    struct peca *peca = ((struct peca *) k);
    if((peca->referencia)->vez == 1)
      jogador = (peca->referencia)->jogador1;
    else
      jogador = (peca->referencia)->jogador2;
  
    if((peca->referencia)->quant_pecasVerdes == 0 || (peca->referencia)->quant_pecasAmarelas == 0)
      str = g_strdup_printf("Fim de Jogo!");
    else
      str = g_strdup_printf("Jogador da Vez: %s", jogador);
    gtk_statusbar_push(GTK_STATUSBAR((peca->referencia)->statusbar), gtk_statusbar_get_context_id(GTK_STATUSBAR((peca->referencia)->statusbar), str), str);
  }
}