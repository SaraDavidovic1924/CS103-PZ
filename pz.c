#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//novi tip adrese
typedef struct adresa
{
    char ulica[40];
    char broj[5];
    char grad[40];
} ADRESA;

//novi tip kontakta (recicemo kontakta jer je cvor_st podaci o kontaktu)
typedef struct cvor_st
{
    char ime_kontakta[50];
    char prezime_kontakta[50];
    char broj_kontakta[30];
    int tip;
    ADRESA adresa[100];

    struct cvor_st *levi;  //levi pokazuje vrednost cvor_st
    struct cvor_st *desni; //desni pokazuje vrednost cvor_st
} BCVOR;

//bira se tip broja Mobilni, fiksni, posao, kuca(tip)
void tip_broja(int tip)
{
    switch (tip)
    {
    case 1:
        printf("Mobilni \n");
        break;
    case 2:
        printf("Fiksni\n");
        break;
    case 3:
        printf("Posao \n");
        break;
    case 4:
        printf("Kuca \n");
        break;
    }
}
//dodavanje u stablo
void dodaj_u_stablo(BCVOR *novi, BCVOR **pkoren)
{
    if (*pkoren == NULL)
    {
        *pkoren = novi;
        novi->levi = NULL;
        novi->desni = NULL;
    }
    else
    {
        //string compare uporedjivanje stringova (karaktera)
        if (strcmp(novi->ime_kontakta, (*pkoren)->ime_kontakta) < 0)
            dodaj_u_stablo(novi, &(*pkoren)->levi);
        else
            dodaj_u_stablo(novi, &(*pkoren)->desni);
    }
}
//ispisivanje kontakta celog kontakta
void ispisi_jedan_kontakt(BCVOR *kontakt)
{
    printf("_______________________________\n");
    printf("Korisnik:\n");
    printf("Ime i prezime %s %s\n", kontakt->ime_kontakta, kontakt->prezime_kontakta);
    printf("Tip telefona: ");
    tip_broja(kontakt->tip);
    printf("Broj telefona: %s\n", kontakt->broj_kontakta);
    printf("Ulica: %s\n", kontakt->adresa->ulica);
    printf("Broj: %s\n", kontakt->adresa->broj);
    printf("Grad: %s\n", kontakt->adresa->grad);
    printf("_______________________________\n");
}
//ispisivanje kontakta iz stabla
void ispisi_stablo(BCVOR *koren) //koren pokazuje na BCVOR sto znaci da ce ispisati cvor_st?
{
    if (koren != NULL)
    {
        ispisi_stablo(koren->levi);
        ispisi_jedan_kontakt(koren);
        ispisi_stablo(koren->desni);
    }
}
//vracanje korisnika po broju telefona
BCVOR *vrati_korisnika_po_broju(BCVOR *koren, char *br)
{
    if (koren != NULL)
    {
        vrati_korisnika_po_broju(koren->levi, br);
        if (strcmp(koren->broj_kontakta, br) == 0)
        {
            return koren;
        }
        vrati_korisnika_po_broju(koren->desni, br);
    }
}
//menjanje korisnika po nekom kriterijumu (bira se kriterijum)
void meni_izmena_koirsnika(BCVOR *root)
{
    char broj[20];
    printf("Ukucajte broj korisnika za azuriranje");
    fflush(stdin);
    scanf("%s", broj);
    fflush(stdin);
    BCVOR *korisnik = vrati_korisnika_po_broju(root, broj);
    printf("\n\t nadjen korisnik pod imenim: %s\n", korisnik->ime_kontakta);
    if (korisnik == NULL)
    {
        printf(" Nema korisnika pod tim brojem");
        return;
    }
    char izmena;
    printf("Sta zelite da menjate kod korisnika?\n[ i za ime, p za prezime, # za broj, u za ulicu, b za broj, g za grad]");
    fflush(stdin);

    scanf("%c", &izmena);
    printf(" Uneli smo %c", izmena);
    fflush(stdin);

    switch (izmena)
    {
    case 'i':
        printf("Novo ime:");
        scanf("%s", korisnik->ime_kontakta);
        printf(" Imenjeni kontakt izgleda ovako:\n");
        ispisi_jedan_kontakt(korisnik);
        break;

    case 'p':
        printf("Novo prezime:");
        scanf("%s", korisnik->prezime_kontakta);
        printf(" Imenjeni kontakt izgleda ovako:\n");
        ispisi_jedan_kontakt(korisnik);
        break;
    case '#':
        printf("Novi broj:");
        scanf("%s", korisnik->broj_kontakta);
        printf(" Imenjeni kontakt izgleda ovako:\n");
        ispisi_jedan_kontakt(korisnik);
        break;

    case 'u':
        printf("Nova ulica:");
        scanf("%s", korisnik->adresa->ulica);
        printf(" Imenjeni kontakt izgleda ovako:\n");
        ispisi_jedan_kontakt(korisnik);
        break;

    case 'b':
        printf("Novi broj ulice:");
        scanf("%s", korisnik->adresa->broj);
        printf(" Imenjeni kontakt izgleda ovako:\n");
        ispisi_jedan_kontakt(korisnik);
        break;

    case 'g':
        printf("Novi grad:");
        scanf("%s", korisnik->adresa->grad);
        printf(" Imenjeni kontakt izgleda ovako:\n");
        ispisi_jedan_kontakt(korisnik);
        break;
    }
}
//ucitavanje podataka iz tektualnog fajla
BCVOR *unesi_podatke_iz_fajla(char *ime_fajla)
{
    FILE *f = fopen(ime_fajla, "r");
    BCVOR *root = NULL;
    BCVOR novi;
    if (f != NULL)
    {
        while (fscanf(f, "%s %s %s %d %s %s %s ",
                      novi.ime_kontakta,
                      novi.prezime_kontakta,
                      novi.broj_kontakta,
                      &novi.tip,
                      novi.adresa->ulica,
                      novi.adresa->broj,
                      novi.adresa->grad) != EOF)
        {
            BCVOR *temp = malloc(sizeof(BCVOR)); //malloc dodeljuje traženu memoriju i vraća pokazivač na nju.
            strcpy(temp->ime_kontakta, novi.ime_kontakta);
            strcpy(temp->prezime_kontakta, novi.prezime_kontakta);
            strcpy(temp->broj_kontakta, novi.broj_kontakta);
            temp->tip = novi.tip;
            strcpy(temp->adresa->ulica, novi.adresa->ulica);
            strcpy(temp->adresa->broj, novi.adresa->broj);
            strcpy(temp->adresa->grad, novi.adresa->grad);

            dodaj_u_stablo(temp, &root);
            printf("_______________________________\n");
            printf("Ime: %s\n", novi.ime_kontakta);
            printf("Prezime: %s\n", novi.prezime_kontakta);
            printf("Ulica: %s\n", novi.adresa->ulica);
        }
        printf("_______________________________\n");
    }
    else
        printf("Nema Fajla.\n");
    fclose(f);
    return root;
}
//cuvanje stabla u fajlu
void sacuvaj_stablo_u_fajl(char *ime_fajla, BCVOR *root)
{

    if (root != NULL)
    {

        FILE *x = fopen(ime_fajla, "a"); //appends
        fprintf(x, "%s %s %s %d %s %s %s\n",
                root->ime_kontakta,
                root->prezime_kontakta,
                root->broj_kontakta,
                root->tip,
                root->adresa->ulica,
                root->adresa->broj,
                root->adresa->grad);

        if (x != NULL)
            fclose(x);
        sacuvaj_stablo_u_fajl(ime_fajla, root->levi);
        sacuvaj_stablo_u_fajl(ime_fajla, root->desni);
    }
}
//Dodavanje novog korisnika
BCVOR novi_podaci()
{
    BCVOR temp;
    //ime
    printf("_______________________________\n");
    printf("Unesite ime korisnika: \n");
    scanf("%s", temp.ime_kontakta);
    printf("_______________________________\n");
    //prezime
    printf("Unesite prezime korisnika: \n");
    scanf("%s", temp.prezime_kontakta);
    printf("_______________________________\n");
    //broj
    printf("Unesite broj korisnika: \n");
    scanf("%s", temp.broj_kontakta);
    printf("_______________________________\n");
    //tip
    printf("Odaberite opciju tipa telefona: \n ");
    printf("-MOBILNI = 1,\n -FIKSNI = 2,\n -POSAO = 3,\n -KUCA = 4\n ");

    scanf("%d", &temp.tip);
    printf("_______________________________\n");
    //adresa
    printf("Unesite adresu korisnika\n");
    printf("--------------------------\n");
    //ulica
    printf("Unesite ulicu korisnika: \n");
    scanf("%s", temp.adresa->ulica);
    printf("_______________________________\n");
    //broj
    printf("Unesite broj adrese korisnika: \n");
    scanf("%s", temp.adresa->broj);
    printf("_______________________________\n");
    //grad
    printf("Unesite grad korisnika: \n");
    scanf("%s", temp.adresa->grad);
    printf("_______________________________\n");
    return temp;
}
//pretraga korisnika po imenu
void nadji_korisnika_po_imenu(BCVOR *imenik, char *ime_kontakta, int *uspehPretrageImena)
{
    if (imenik != NULL)
    {
        nadji_korisnika_po_imenu(imenik->levi, ime_kontakta, uspehPretrageImena);
        if (strcmp(ime_kontakta, imenik->ime_kontakta) == 0) //kada je jednako nula to znaci da je pronadjen kontakt
        {
            ispisi_jedan_kontakt(imenik);
            *uspehPretrageImena = 1;
        }

        nadji_korisnika_po_imenu(imenik->desni, ime_kontakta, uspehPretrageImena);
    }
}
//pretraga korisnika po broju
void nadji_korisnika_po_broju(BCVOR *imenik, char *broj_kontakta, int *uspehPretrage)
{
    if (imenik != NULL)
    {
        nadji_korisnika_po_broju(imenik->levi, broj_kontakta, uspehPretrage);

        if (strcmp(broj_kontakta, imenik->broj_kontakta) == 0)
        {
            ispisi_jedan_kontakt(imenik);
            *uspehPretrage = 1;
        }
        nadji_korisnika_po_broju(imenik->desni, broj_kontakta, uspehPretrage);
    }
}
//minimalna vrednost cvora (kada je minimalna vrednost cvora, onda se bcvor ili trenutni cvor nalazi u levoj strani)
BCVOR *minValueNode(BCVOR *node)
{
    BCVOR *current = node;

    while (current->levi != NULL)
        current = current->levi;
    return current;
}

// Brisanje korisnika (brisanjem cvora brise se korisnik)
BCVOR *deleteNode(BCVOR *root, char *key, int *uspeh)
{
    if (root == NULL)
        return root;
    printf(" %s, %s\n", key, root->broj_kontakta);

    //kada je uporedjivanje char-ova, gleda se koji je veci i na osnovu toga se prelazi na levu ili desnu stranu stabla
    if (strcmp(key, root->broj_kontakta) < 0)
        root->levi = deleteNode(root->levi, key, uspeh); //brisanje iz levog cvora

    else if (strcmp(key, root->broj_kontakta) > 0)
        root->desni = deleteNode(root->desni, key, uspeh); //brisanje iz desnog cvora
    else
    {

        if (root->levi == NULL)
        {
            BCVOR *temp = root->desni;
            free(root);
            *uspeh = 1;
            return temp;
        }

        else if (root->desni == NULL)
        {
            BCVOR *temp = root->levi;
            free(root);
            *uspeh = 1;
            return temp;
        }
        BCVOR *temp = minValueNode(root->desni);
        strcpy(root->broj_kontakta, temp->broj_kontakta);
        root->desni = deleteNode(root->desni, temp->broj_kontakta, uspeh);
    }
    return root;
}
//pokretacka funkcija
int main()
{ //Ovo je zapravo root, ili koren stabla
    BCVOR *imenik = NULL;
    imenik = unesi_podatke_iz_fajla("korisnici.txt");
    int opcija;

    do
    {
        printf("[1.] Pretraga korisnika na osnovu imena\n");
        printf("[2.] Pretraga korisnika na osnovu broja\n");
        printf("[3.] Ispis stabla\n");
        printf("[4.] Unos novog clana\n");
        printf("[5.] Upis stabla u fajl\n");
        printf("[6.] Brisanje cvora po broju telefona\n");
        printf("[7.] Azuriranje postojeceg korisnika\n");

        scanf("%d", &opcija);

        char zadato_ime[100];
        int unos;
        char zadati_broj[40];
        int uspehBrisanja = 0;
        int uspehPretrage = 0;
        int uspehImena = 0;

        BCVOR novi;
        FILE *input = NULL;

        switch (opcija)
        {
        case 1:
            printf("Unesite ime za pretragu:\n");
            scanf("%s", zadato_ime);
            nadji_korisnika_po_imenu(imenik, zadato_ime, &uspehImena);
            if (uspehImena == 0)
                printf("Korisnik sa ovim imenom ne postoji!\n");
            else
                printf("Korisnik postoji!\n");
            printf("_______________________________\n");
            break;

        case 2:
            printf("Unesite broj telefona za pretragu: \n");
            scanf("%s", zadati_broj);
            fflush(stdin);
            nadji_korisnika_po_broju(imenik, zadati_broj, &uspehPretrage);
            if (uspehPretrage == 0)
                printf("Korisik nije pronadjen!\n");
            else
                printf("Korisnik je pronadjen!\n");
            printf("_______________________________\n");
            uspehPretrage = 1;
            break;

        case 3:
            ispisi_stablo(imenik);
            break;

        case 4:
            novi = novi_podaci();
            dodaj_u_stablo(&novi, &imenik);
            break;

        case 5:
            printf(" Da li zelite u isti fajl da upisete?\n[korisnici.txt]\n[1/0]");
            scanf("%d", &unos);
            if (unos != 0)
            {
                printf("rekli y");
                input = fopen("korisnici.txt", "w");
                fclose(input);
                sacuvaj_stablo_u_fajl("korisnici.txt", imenik);
            }
            break;

        case 6:
            printf("Unesite broj telefona za brisanje: \n");
            scanf("%s", zadati_broj);
            fflush(stdin);
            imenik = deleteNode(imenik, zadati_broj, &uspehBrisanja);

            if (uspehBrisanja == 0)
                printf("Korisnik nije pronadjen!\n");
            else
                printf("Korisnik je obrisan!\n");
            uspehBrisanja = 0;
            break;
        case 7:
            meni_izmena_koirsnika(imenik);
            break;
        }
    } while (opcija != 0);
}