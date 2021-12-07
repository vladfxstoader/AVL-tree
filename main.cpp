#include <bits/stdc++.h>
using namespace std;

fstream in1, in2, out1, out2;

// Multimea propriu-zisa, generata si stocata prin intermediul unui struct
struct Nod
{
    long long cheie;
    Nod* stanga;
    Nod* dreapta;
    int inaltime; // Inaltimea arborelui care are ca radacina un anumit nod
    static long long nrElemente;

    Nod(long long Cheie): cheie(Cheie), stanga(nullptr), dreapta(nullptr), inaltime(1){}

    long long k_element(long long k)
    {
        --k; // Elementele multimii sunt indexate de la 0
        long long contor = 0;
        long long kElement = LLONG_MIN;
        Nod* nodCurent = this;

        while (nodCurent)
        {
            if (!nodCurent->stanga)
            {
                ++contor;
                if (contor == k+1)
                    kElement = nodCurent->cheie;

                nodCurent = nodCurent->dreapta;
            }
            else
            {
                Nod* predecInord = nodCurent->stanga;
                while ((predecInord->dreapta) && (predecInord->dreapta != nodCurent))
                    predecInord = predecInord->dreapta;

                if (!predecInord->dreapta)
                {
                    predecInord->dreapta = nodCurent;
                    nodCurent = nodCurent->stanga;
                }
                else
                {
                    predecInord->dreapta = nullptr;

                    ++contor;
                    if (contor == k+1)
                        kElement = nodCurent->cheie;
                    nodCurent = nodCurent->dreapta;
                }
            }
        }

        return kElement;
    }
};

long long Nod::nrElemente = 0;

// Functie care afla inaltimea unui arbore
int InaltimeArbore(Nod* nod)
{
    if (!nod)
        return 0;

    return nod->inaltime;
}

// Functie care calculeaza maximul dintre doua valori
long long Maxim(long long a, long long b)
{
    return (a > b)? a : b;
}

// Functie care realizeaza o rotatie simpla stanga pe un arbore
Nod* RotatieStanga(Nod* x)
{
    Nod* y = x->dreapta;
    Nod* z = y->stanga;

    y->stanga = x;
    x->dreapta = z;

    x->inaltime = Maxim(InaltimeArbore(x->stanga), InaltimeArbore(x->dreapta))+1;
    y->inaltime = Maxim(InaltimeArbore(y->stanga), InaltimeArbore(y->dreapta))+1;

    return y;
}

// Functie care realizeaza o rotatie simpla dreapta pe un arbore
Nod* RotatieDreapta(Nod* x)
{
    Nod* y = x->stanga;
    Nod* z = y->dreapta;

    y->dreapta = x;
    x->stanga = z;

    x->inaltime = Maxim(InaltimeArbore(x->stanga), InaltimeArbore(x->dreapta))+1;
    y->inaltime = Maxim(InaltimeArbore(y->stanga), InaltimeArbore(y->dreapta))+1;

    return y;
}

// Functie care realizeaza afisarea arborelui in inordine
void AfisareArboreInord(Nod* nod)
{
    if (nod)
    {
        AfisareArboreInord(nod->stanga);
        out1<<nod->cheie<<'\n';
        AfisareArboreInord(nod->dreapta);
    }
}

// Functie care calculeaza factorul de echilibrare al unui nod
int AflareFactorEchilibrare(Nod* nod)
{
    return InaltimeArbore(nod->stanga)-InaltimeArbore(nod->dreapta);
}

Nod* insereaza(Nod* nod, long long cheie)
{
    if (!nod)
    {
        ++Nod::nrElemente;
        Nod* nodNou = new Nod(cheie);
        return nodNou;
    }
    else
        if (cheie < nod->cheie)
            nod->stanga = insereaza(nod->stanga, cheie);
        else
            if (cheie > nod->cheie)
                nod->dreapta = insereaza(nod->dreapta, cheie);
            else
                return nod; // Multimile au doar elemente distincte

    nod->inaltime = Maxim(InaltimeArbore(nod->stanga), InaltimeArbore(nod->dreapta))+1; // Actualizam inaltimea arborelui care il are ca radacina pe "nod"

    int balance = AflareFactorEchilibrare(nod);

    // Realizam echilibrarea arborelui, daca este cazul:

    // Cazul in care trebuie sa efectuam o rotatie simpla stanga
    if ((balance < -1) && (cheie > nod->dreapta->cheie))
        return RotatieStanga(nod);

    // Cazul in care trebuie sa efectuam o rotatie dubla stanga (o rotatie simpla dreapta + o rotatie simpla stanga)
    if ((balance < -1) && (cheie < nod->dreapta->cheie))
    {
        nod->dreapta = RotatieDreapta(nod->dreapta);
        return RotatieStanga(nod);
    }

    // Cazul in care trebuie sa efectuam o rotatie simpla dreapta
    if ((balance > 1) && (cheie < nod->stanga->cheie))
        return RotatieDreapta(nod);

    // Cazul in care trebuie sa efectuam o rotatie dubla dreapta (o rotatie simpla stanga + o rotatie simpla dreapta)
    if ((balance > 1) && (cheie > nod->stanga->cheie))
    {
        nod->stanga = RotatieStanga(nod->stanga);
        return RotatieDreapta(nod);
    }

    return nod;
}

// Functie care returneaza nodul cu cea mai mica cheie dintr-un anumit arbore
Nod* AflareNodMinim(Nod* nod)
{
    Nod* nodCurent = nod;

    while (nodCurent->stanga)
        nodCurent = nodCurent->stanga;

    return nodCurent;
}

Nod* sterge(Nod* nod, long long cheie)
{
    if (!nod)
        return nod;
    else
        if (cheie < nod->cheie)
            nod->stanga = sterge(nod->stanga, cheie);
        else
            if(cheie > nod->cheie)
                nod->dreapta = sterge(nod->dreapta, cheie);
            else
            {
                // Verificam daca nodul are un singur fiu sau niciunul, sau daca are 2 fii
                if((!nod->stanga) || (!nod->dreapta))
                {
                    Nod* nodAux = (nod->stanga != nullptr)? nod->stanga : nod->dreapta;

                    // Cazul in care nodul nu are niciun fiu
                    if (!nodAux)
                    {
                        --Nod::nrElemente; // Actualizam cardinalul multimii
                        nodAux = nod;
                        nod = nullptr;
                    }
                    else // Cazul in care nodul are un singur fiu
                        {
                            --Nod::nrElemente; // Actualizam cardinalul multimii
                            *nod = *nodAux;
                        }

                    delete nodAux;
                }
                else // Cazul in care nodul are 2 fii
                {
                    Nod* nodAux = AflareNodMinim(nod->dreapta);
                    nod->cheie = nodAux->cheie;
                    nod->dreapta = sterge(nod->dreapta, nodAux->cheie);
                }
            }

    if (!nod)
        return nod;

    nod->inaltime = Maxim(InaltimeArbore(nod->stanga), InaltimeArbore(nod->dreapta))+1; // Actualizam inaltimea arborelui care il are ca radacina pe "nod"

    int balance = AflareFactorEchilibrare(nod);

    // Realizam echilibrarea arborelui, daca este cazul:

    // Cazul in care trebuie sa efectuam o rotatie simpla stanga
    if ((balance < -1) && (AflareFactorEchilibrare(nod->dreapta) <= 0))
        return RotatieStanga(nod);

    // Cazul in care trebuie sa efectuam o rotatie dubla stanga (o rotatie simpla dreapta + o rotatie simpla stanga)
    if ((balance < -1) && (AflareFactorEchilibrare(nod->dreapta) > 0))
    {
        nod->dreapta = RotatieDreapta(nod->dreapta);
        return RotatieStanga(nod);
    }

    // Cazul in care trebuie sa efectuam o rotatie simpla dreapta
    if ((balance > 1) && (AflareFactorEchilibrare(nod->stanga) >= 0))
        return RotatieDreapta(nod);

    // Cazul in care trebuie sa efectuam o rotatie dubla dreapta (o rotatie simpla stanga + o rotatie simpla dreapta)
    if ((balance > 1) && (AflareFactorEchilibrare(nod->stanga) < 0))
    {
        nod->stanga = RotatieStanga(nod->stanga);
        return RotatieDreapta(nod);
    }

    return nod;
}

int este_in(Nod* nod, long long cheie)
{
    if (!nod)
        return 0;
    else // Parcurgem arborele ca pe un arbore binar de cautare
        if (cheie < nod->cheie)
            return este_in(nod->stanga, cheie);
        else
            if (cheie > nod->cheie)
                return este_in(nod->dreapta, cheie);
            else
                return 1;
}

long long min(Nod* nod)
{
    // Cazul in care arborele este gol
    if (!nod)
        throw runtime_error("Arborele dat este gol!\n");

    // Elementul minim este cel mai din stanga element al arborelui
    if (!nod->stanga)
        return nod->cheie;
    else
        return min(nod->stanga);
}

long long max(Nod* nod)
{
    // Cazul in care arborele este gol
    if (!nod)
        throw runtime_error("Arborele dat este gol!\n");

    // Elementul maxim este cel mai din dreapta element
    if (!nod->dreapta)
        return nod->cheie;
    else
        return max(nod->dreapta);
}

long long predecesor (Nod* nod, long long cheie)
{
    long long predec = LLONG_MIN;

    while (true)
    {
        if (cheie < nod->cheie) // Daca cheia data e mai mica decat radacina, mergem in subarborele stang
            nod = nod->stanga;
        else
            if (cheie > nod->cheie) // Daca cheia data e mai mare decat radacina, mergem in subarborele drept
            {
                // Actualizam predecesorul la nodul curent inainte de a vizita subarborele drept
                predec = nod->cheie;
                nod = nod->dreapta;
            }
            else // Predecesorul va fi maximul din subarborele stang (daca exista)
            {
                if (nod->stanga)
                    predec = max(nod->stanga);

                break;
            }

        // Cazul in care cheia nu exista in arbore
        if (!nod)
            throw runtime_error("Nodul dat nu se afla in arbore!");
    }

    if (predec != LLONG_MIN)
        return predec;
    else
        throw runtime_error("Nodul dat nu are predecesor!");
}

long long succesor(Nod* nod, long long cheie)
{
    long long succ = LLONG_MAX;
    while (true)
    {
        if (cheie < nod->cheie) // Daca cheia data e mai mare decat radacina, vizitam subarborele stang
        {
            // Actualizam succesorul la nodul curent inainte de a vizita subarborele stang
            succ = nod->cheie;
            nod = nod->stanga;
        }
        else
            if (cheie > nod->cheie) // Daca cheia data e mai mare decat radacina, vizitam subarborele drept
                nod = nod->dreapta;
            else // Succesorul va fi minimul valorilor din subarborele drept (daca exista)
            {
                if (nod->dreapta)
                    succ = min(nod->dreapta);

                break;
            }

        // Cazul in care cheia nu exista in arbore
        if (!nod)
            throw runtime_error("Nodul dat nu se afla in arbore!");
    }

    if (succ != LLONG_MAX)
        return succ;
    else
        throw runtime_error("Nodul dat nu are succesor!");
}

long long cardinal(Nod *nod)
{
    return Nod::nrElemente;
}

/// Pentru a se genera noi teste, cu numere aleatorii, se apeleaza functia "GenerareTest", din interiorul functiei "VerificaFisiere"
bool GenerareTest();

void VerificaFisiere()
{
    long long n, k, randomEsteIn, x;
    Nod *radac = nullptr;

    /// Aceasta sectiune trebuie decomentata doar daca se doreste generarea de teste noi
    /**
    bool t = GenerareTest();
    if (!t)
        return;
    **/

    in1.open("random_order.txt", ios::in);
    in1>>n>>k>>randomEsteIn;

    while (in1>>x)
        radac = insereaza(radac,x);

    in1.close();

    out1.open("output_avl.txt", ios::out);

    out1<<cardinal(radac)<<'\n';
    cout<<"Cardinalul multimii: "<<cardinal(radac)<<'\n';

    AfisareArboreInord(radac);

    long long kthElem = radac->k_element(k);

    out1<<kthElem<<'\n';
    cout<<"Al "<<k<<"-lea element din multime: "<<kthElem<<'\n';

    out1<<randomEsteIn<<" "<<este_in(radac, randomEsteIn)<<'\n';
    cout<<"Numarul cu care testam \"este_in\": "<<randomEsteIn<<". Se afla acesta in arbore? (0 - Nu, 1 - Da) "<<este_in(radac, randomEsteIn)<<'\n';

    try
    {
        out1<<min(radac)<<" "<<max(radac)<<'\n';
        cout<<"Cea mai mica valoare din arbore: "<<min(radac)<<" || Cea mai mare valoare din arbore: "<<max(radac)<<'\n';
    }
    catch (exception& e)
    {
        cout<<e.what();
    }

    try
    {
        out1<<predecesor(radac, kthElem)<<" ";
        cout<<"Predecesorul lui "<<kthElem<<": "<<predecesor(radac, kthElem);
    }
    catch (exception& e)
    {
        cout<<e.what();
    }

    cout<<" || ";

    try
    {
        out1<<succesor(radac, kthElem)<<'\n';
        cout<<"Succesorul lui "<<kthElem<<": "<<succesor(radac, kthElem)<<'\n';
    }
    catch (exception& e)
    {
        cout<<e.what()<<'\n';
    }

    in1.open("f_delete.txt", ios::in);

    long long z;
    while (in1>>z)
           radac = sterge(radac, z);

    cout<<"Nr elemente dupa stergere: "<<cardinal(radac)<<'\n';

    in1.close();

    AfisareArboreInord(radac);

    out1.close();

    // Comparam fisierele "output_avl" si "sorted_order"
    in1.open("output_avl.txt", ios::in);
    in2.open("sorted_order.txt", ios::in);

    long long x1, x2;
    cout<<"--------------------------\n";
    while ((in1>>x1) && (in2>>x2))
    if (x1 != x2)
    {
        cout<<"OUTPUT-URI DIFERITE!\n";
        return;
    }
    cout<<"OUTPUT-URI EGALE!\n";
    cout<<"--------------------------\n";

    cout<<"Inaltimea arborelui: "<<radac->inaltime<<'\n';

    in1.close();
    in2.close();
}

int main()
{

VerificaFisiere();

return 0;}

/// Pentru a se genera noi teste, cu numere aleatorii, se apeleaza functia "GenerareTest", din interiorul functiei "VerificaFisiere"
bool GenerareTest()
{
    long long n, x, x2, i;

    out1.open("random_order.txt", ios::out);

    list<long long> ls;

    srand(time(0));
    n = rand(); // Numarul initial de elemente din multime

    if (!n)
        return false;

    long long k = rand()%(n+1);

    long long randomEsteIn = rand();

    out1<<n<<' '<<k<<' '<<randomEsteIn<<'\n';
    cout<<"Nr elemente: "<<n<<" || "<<"Pozitia, not cu k, pe care se afla al k-lea element din multime: "<<k<<" || Numarul cu care testam \"este_in\": "<<randomEsteIn<<'\n';

    // Generam nodurile arborelui
    i = 0;
    while (i < n)
    {
        x = rand();
        // Verificam daca nodul generat (care are valoarea x) se afla deja in arbore. Daca
        // nu se afla in arbore, il adaugam, daca se afla, nu il adaugam din nou
        list<long long>::iterator findIter = find(ls.begin(), ls.end(), x);
        if (findIter == ls.end())
        {
            ls.push_back(x);
            ++i;
        }
    }

    // Adaugam nodurile arborelui in fisierul "random_order.txt"
    for (auto it = ls.begin(); it != ls.end(); ++it)
        out1<<(*it)<<'\n';

    out1.close();

    out1.open("sorted_order.txt", ios::out);

    out1<<n<<'\n';

    // Adaugam nodurile arborelui in fisierul "sorted_order.txt", in ordinea crescatoare a cheilor
    set<long long> lsOrdered;
    for (auto it=ls.begin(); it != ls.end(); ++it)
        lsOrdered.insert((*it));
    for (auto it = lsOrdered.begin();it != lsOrdered.end(); ++it)
        out1<<(*it)<<'\n';


    set<long long>::iterator it = lsOrdered.begin();
    advance(it, k-1); // Avanseaza automat la pozitia la care se afla al k-lea element din multime
    x = *it;
    x2 = x;
    out1<<x<<'\n';
    cout<<"Al "<<k<<"-lea element din multime: "<<x<<'\n';

    out1<<randomEsteIn<<" ";
    cout<<"Numarul cu care testam \"este_in\": "<<randomEsteIn<<". Se afla acesta in arbore? (0 - Nu, 1 - Da) ";
    set<long long>::iterator findIter = find(lsOrdered.begin(), lsOrdered.end(), randomEsteIn);
    if (findIter == lsOrdered.end())
    {
        out1<<0<<'\n';
        cout<<0<<'\n';
    }
    else
    {
        out1<<1<<'\n';
        cout<<1<<'\n';
    }

    out1<<(*(lsOrdered.begin()))<<" "<<(*(--lsOrdered.end()))<<'\n';
    cout<<"Cea mai mica valoare din arbore: "<<(*(lsOrdered.begin()))<<" || Cea mai mare valoare din arbore: "<<(*(--lsOrdered.end()))<<'\n';

    // Verificam daca al k-lea element are predecesor, caz in care afisam predecesorul acestuia
    if (k < 2)
        cout<<"Al "<<k<<"-lea element nu are predecesor! || ";
    else
    {
        cout<<"Predecesorul lui "<<x<<": ";
        it = lsOrdered.begin();
        advance(it, k-2);
        x = *it;
        out1<<x<<" ";
        cout<<x<<" || ";
    }

    // Verificam daca al k-lea element are succesor, caz in care afisam succesorul acestuia
    if (k >= n)
        cout<<"Al "<<k<<"-lea element nu are succesor! || ";
    else
    {
        cout<<"Succesorul lui "<<x2<<": ";
        it = lsOrdered.begin();
        advance(it, k);
        x = *it;
        out1<<x<<'\n';
        cout<<x<<'\n';
    }

    long long numbersToDelete = rand()%(n+1);

    long long poz;

    list<long long> pozitii;

    out2.open("f_delete.txt", ios::out);

    i = 0;
    long long cn = n;
    while (i < numbersToDelete)
    {
        poz = rand()%cn+1;
        list<long long>::iterator findIter = find(pozitii.begin(), pozitii.end(), poz);
        if (findIter == pozitii.end())
        {
            pozitii.push_back(poz);
            ++i;
            --cn;
            set<long long>::iterator it = lsOrdered.begin();
            advance(it, poz-1); // Avanseaza automat la pozitia poz
            long long x = *it;
            out2<<x<<'\n';
            lsOrdered.erase(x);
        }
    }

    out2.close();

    for (auto it = lsOrdered.begin(); it != lsOrdered.end(); ++it)
        out1<<(*it)<<'\n';

    out1.close();

    cout<<n<<" elemente initiale, "<<numbersToDelete<<" elemente sterse, "<<lsOrdered.size()<<" elemente ramase\n";
    cout<<"Nr elemente dupa stergere: "<<n-numbersToDelete<<'\n';

    cout<<"--------------------------\n";

    return true;
}
