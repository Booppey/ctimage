#include "control.h"
#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

void displayWelcome()
{
  cout<<"Bienvenue d'utiliser ctimage - l'auteur: NGUYEN Quoc Khai\n";
  cout<<"======================================\n";
}

void displayMenu()
{
  cout<<"Pourriez-vous taper:\n";
  cout<<"prof [option] image x1 y1 x2 y2 - pour afficher le profil\n";
  cout<<"cont [option] image function - pour caculer le contraste\n";
  cout<<"\toption: 0 pour image gray : 1 pour image de coleur\n";
  cout<<"\tfunction: 1, 2, 3, 4 ou 5\n";
  cout<<"\t\t1: Linaire\n";
  cout<<"\t\t2: Linaire avec saturation\n";
  cout<<"\t\t3: Linaire par morceaux\n";
  cout<<"\t\t4: Correction gama\n";
  cout<<"\t\t5: Egalisation de l'histogramme\n";
  cout<<"======================================\n";
}

int main(int argc, char* argv[])
{
  Control control;
  int x1, y1, x2, y2;
  string cmd("");
  int option = 0;
  string image("image.jpg");
  int function = 1;
  string exit = "no";

  displayWelcome();
  do
    {
      displayMenu();
      cin>>cmd >>option >>image;
      if(cmd.compare("prof") == 0)
	{
	  cin>>x1 >>y1 >>x2 >>y2;
	  control.drawProfil(option, image, x1, y1, x2, y2);
	}
      else if(cmd.compare("cont") == 0)
	{
	  cin>>function;
	  control.calcContrast(option, image, function);
	}
      else{
	cout<<"Aucune commande trouvée\n";
	continue;
      }
      cout<<"Trouvez les résultats dans image/\n";
      cout<<"yes - pour quitter ctimage: \n";
      cout<<"no - pour continuer d'utiliser ctimage: \n";
      cin>>exit;
    } while (exit.compare("yes") != 0);
  return 0;
}
