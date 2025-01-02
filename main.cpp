#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <limits>
#include <cmath>
#include <chrono>
#include <cstring>
#include <functional>
#include <fstream>

using namespace std;
const double INF = numeric_limits<double>::infinity();

// prototypy
void readConfig(string& nazwa_pliku,int& iteracje,int& wybor_algorytmu);
vector<vector<double>> parseTXT(string& nazwa_pliku, double& wynikOptymalny);
double OldTSPBranchAndBound(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna);
double TSPBestFirstSearch(vector<vector<double>>& graf, int start, vector<int>& sciezka_minimalna, double wartoscOptymalna);
double TSPBranchAndBound(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna);
//double TSPBestFirstSearch(vector<vector<double>>& graf, int start, vector<int>& sciezka_minimalna, double wartoscOptymalna);
double TSPAStar(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna);
double TSPNearestNeighbor(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna);


int main() {
//    vector<vector<double>> graf =  {
//            {0, 8532, 1512, 2146, 6321, 6132, 3821, 8213, 5328, 2145},
//            {1622, 0, 4426, 8630, 8423, 4291, 9328, 6238, 6328, 5132},
//            {5263, 3521, 0, 5124, 2416, 5326, 3128, 3219, 4213, 6125},
//            {2821, 3521, 2152, 0, 7243, 3912, 6328, 2419, 6219, 2140},
//            {3641, 6321, 3012, 2314, 0, 5322, 3928, 9542, 2140, 8431},
//            {7221, 3521, 2152, 9542, 6727, 0, 9231, 5294, 2137, 5126},
//            {5326, 6328, 7231, 7421, 6323, 3219, 0, 3912, 9214, 8329},
//            {7523, 2147, 7234, 3742, 4281, 4215, 6878, 0, 4218, 4312},
//            {6313, 7321, 3273, 8243, 6900, 2155, 8008, 1320, 0, 6321},
//            {2145, 2146, 6234, 4289, 8429, 9231, 5241, 7328, 7231, 0}
//    };

    string czesc_sciezki = "../data/";
    string czesc_plikowa;
    string nazwa_pliku; // nazwa pliku z grafem
    int iteracje;
    int wybor_algorytmu;

    readConfig(nazwa_pliku, iteracje, wybor_algorytmu);
    czesc_plikowa = nazwa_pliku;
    nazwa_pliku = czesc_sciezki + czesc_plikowa;

    double wynikOptymalny;

    vector<vector<double>> graf = parseTXT(nazwa_pliku, wynikOptymalny);

    int start = 0; //wierzcholek poczatkowy
    vector<int> wierzcholkiAStar;
    vector<int> wierzcholkiBranchAndBound;
    vector<int> wierzcholkiBestFirstSearch;

    double wynik_BranchAndBound = 0;
    double wynik_BestFirstSearch = 0;
    double wynik_AStar = 0;

    auto start_BranchAndBound = chrono::high_resolution_clock::now();
    auto end_BranchAndBound = chrono::high_resolution_clock::now();
    auto czas_BranchAndBound = chrono::duration_cast<chrono::milliseconds>(end_BranchAndBound - start_BranchAndBound).count();

    auto start_BestFirstSearch = chrono::high_resolution_clock::now();
    auto end_BestFirstSearch = chrono::high_resolution_clock::now();
    auto czas_BestFirstSearch = chrono::duration_cast<chrono::milliseconds>(end_BestFirstSearch - start_BestFirstSearch).count();

    auto start_AStar = chrono::high_resolution_clock::now();
    auto end_AStar = chrono::high_resolution_clock::now();
    auto czas_AStar = chrono::duration_cast<chrono::milliseconds>(end_AStar - start_AStar).count();

    double blad;

    ofstream plik_wynikowy("results.txt");
    ofstream plik_wynikowy_czasow("results_times.txt");
    ofstream plik_wynikowy_kosztow("results_costs.txt");

    switch (wybor_algorytmu) {
        case 1: // BranchAndBound
            for(int ite =0; ite < iteracje; ite++){
                start_BranchAndBound = chrono::high_resolution_clock::now();
                wynik_BranchAndBound = TSPBranchAndBound(graf, start, wierzcholkiBranchAndBound, wynikOptymalny);
                end_BranchAndBound = chrono::high_resolution_clock::now();

                double blad = ((wynik_BranchAndBound - wynikOptymalny)/wynik_BranchAndBound) * 100;

                for(int i = 0; i< graf.size()+1; i++){
                    cout << wierzcholkiBranchAndBound[i] << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki BNB: " << wynik_BranchAndBound << endl;

                czas_BranchAndBound = chrono::duration_cast<chrono::milliseconds>(end_BranchAndBound - start_BranchAndBound).count();
                cout << czas_BranchAndBound << " ms" << endl;
                plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "BranchAndBound" << " "<< ite + 1 << " " << wynik_BranchAndBound << " " << czas_BranchAndBound << endl;
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchAndBound[it] << " ";} plik_wynikowy << endl;// sciezka
                plik_wynikowy_czasow << czesc_plikowa << " " << "BranchAndBound" << " " << czas_BranchAndBound << endl;
                plik_wynikowy_kosztow << czesc_plikowa << " " << "BranchAndBound" << " " << wynik_BranchAndBound << " " << wynikOptymalny << " " << blad << endl;

// bedzie plik results.txt w ktorym bedzie wszystko, taki chaos jak dotychczas
//results_costs.txt w ktorym beda tylko: plik z grafem, algorytm, koszt znaleziony, koszt optymalny, blad (znalezione-optymalne/znalezione * 100%)
//                z results_costs.txt możnaby wtedy tworzyć grafy
//                results_times.txt w ktorym beda: plik z grafem, algorytm, czas wykonania
            }

            plik_wynikowy << " " << endl;
            plik_wynikowy << " " << endl;
            plik_wynikowy << "Jak odczytywac plik wynikowy?" << endl;
            plik_wynikowy << "Nazwa pliku z grafem" << endl;
            plik_wynikowy << "Rozmiar grafu" << endl;
            plik_wynikowy << "Algorytm" << endl;
            plik_wynikowy << "Ktora iteracja" << endl;
            plik_wynikowy << "Koszt sciezki" << endl;
            plik_wynikowy << "Czas wykonania" << endl;
            plik_wynikowy << "Sciezka" << endl;

            plik_wynikowy.close();
            break;
        case 2: // BestFirstSearch
            for(int ite = 0; ite < iteracje; ite++){
                start_BestFirstSearch = chrono::high_resolution_clock::now();
                wynik_BestFirstSearch = TSPBestFirstSearch(graf, start, wierzcholkiBestFirstSearch, wynikOptymalny);
                end_BestFirstSearch = chrono::high_resolution_clock::now();

                for (int i : wierzcholkiBestFirstSearch) {
                    cout << i << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki BestFirstSearch: " << wynik_BestFirstSearch << endl;

                cout << wynik_BestFirstSearch << endl;
                czas_BestFirstSearch = chrono::duration_cast<chrono::milliseconds>(end_BestFirstSearch - start_BestFirstSearch).count();
                cout << czas_BestFirstSearch << " ms" << endl;
                plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "BestFirstSearch" << " " << ite + 1 << " " << wynik_BestFirstSearch << " " << czas_BestFirstSearch << endl;
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBestFirstSearch[it] << " ";} plik_wynikowy << endl; // sciezka
                plik_wynikowy_czasow << czesc_plikowa << " " << "BranchAndBound" << " " << czas_BestFirstSearch << endl;
                plik_wynikowy_kosztow << czesc_plikowa << " " << "BranchAndBound" << " " << wynik_BestFirstSearch << " " << wynikOptymalny << " " << blad << endl;


            }

                            plik_wynikowy << "" << endl;
                plik_wynikowy << "" << endl;
                plik_wynikowy << "Jak odczytywac plik wynikowy?" << endl;
                plik_wynikowy << "Nazwa pliku z grafem" << endl;
                plik_wynikowy << "Rozmiar grafu" << endl;
                plik_wynikowy << "Algorytm" << endl;
                plik_wynikowy << "Ktora iteracja" << endl;
                plik_wynikowy << "Koszt sciezki" << endl;
                plik_wynikowy << "Czas wykonania" << endl;
                plik_wynikowy << "Sciezka" << endl;

            plik_wynikowy.close();
            break;
        case 3: // AStar
            for(int ite = 0 ; ite < iteracje; ite++){
                start_AStar = chrono::high_resolution_clock::now();
                wynik_AStar = TSPAStar(graf, start, wierzcholkiAStar, wynikOptymalny);
                end_AStar = chrono::high_resolution_clock::now();

                for(int i = 0; i< graf.size()+1; i++){
                    cout << wierzcholkiAStar[i] << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki AStar: " << wynik_AStar << endl;

                czas_AStar = chrono::duration_cast<chrono::milliseconds>(end_AStar - start_AStar).count();

                cout << czas_AStar << " ms" << endl;

                //plik_wynikowy.open("results.txt");

//                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
//                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
//                plik_wynikowy << "AStar" << endl;                  // jaki algorytm
//                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
//                plik_wynikowy << wynik_AStar << endl;                // koszt sciezki
//                plik_wynikowy << czas_AStar  << endl;                // czas wykonania
//                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiAStar[it] << " ";} plik_wynikowy << endl; // sciezka

                plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "AStar" << " " << ite + 1 << " " << wynik_AStar << " " << czas_AStar << endl;
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiAStar[it] << " ";} plik_wynikowy << endl; // sciezka

                plik_wynikowy_czasow << czesc_plikowa << " " << "AStar" << " " << czas_AStar << endl;

                plik_wynikowy_kosztow << czesc_plikowa << " " << "AStar" << " " << wynik_AStar << " " << wynikOptymalny << " " << blad << endl;


            }

            plik_wynikowy << "" << endl;
                plik_wynikowy << "" << endl;
                plik_wynikowy << "Jak odczytywac plik wynikowy?" << endl;
                plik_wynikowy << "Nazwa pliku z grafem" << endl;
                plik_wynikowy << "Rozmiar grafu" << endl;
                plik_wynikowy << "Algorytm" << endl;
                plik_wynikowy << "Ktora iteracja" << endl;
                plik_wynikowy << "Koszt sciezki" << endl;
                plik_wynikowy << "Czas wykonania" << endl;
                plik_wynikowy << "Sciezka" << endl;

            plik_wynikowy.close();
            break;
        case 4:
            for(int ite = 0 ; ite < iteracje; ite++){
                start_BranchAndBound = chrono::high_resolution_clock::now();
                wynik_BranchAndBound = TSPBranchAndBound(graf, start, wierzcholkiBranchAndBound, wynikOptymalny);
                end_BranchAndBound = chrono::high_resolution_clock::now();

                for(int i = 0; i< graf.size()+1; i++){
                    cout << wierzcholkiBranchAndBound[i] << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki BNB: " << wynik_BranchAndBound << endl;

                czas_BranchAndBound = chrono::duration_cast<chrono::milliseconds>(end_BranchAndBound - start_BranchAndBound).count();
                cout << czas_BranchAndBound << " ms" << endl;

                //////////////////////////////////////////////////////////////////////////////////////////////////
                cout << "" << endl;
                //////////////////////////////////////////////////////////////////////////////////////////////////

                start_BestFirstSearch = chrono::high_resolution_clock::now();
                wynik_BestFirstSearch = TSPBestFirstSearch(graf, start, wierzcholkiBestFirstSearch, wynikOptymalny);
                end_BestFirstSearch = chrono::high_resolution_clock::now();

                for (int i : wierzcholkiBestFirstSearch) {
                    cout << i << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki BestFirstSearch: " << wynik_BestFirstSearch << endl;

                czas_BestFirstSearch = chrono::duration_cast<chrono::milliseconds>(end_BestFirstSearch - start_BestFirstSearch).count();
                cout << czas_BestFirstSearch << " ms" << endl;

                //////////////////////////////////////////////////////////////////////////////////////////////////
                cout << "" << endl;
                //////////////////////////////////////////////////////////////////////////////////////////////////

                start_AStar = chrono::high_resolution_clock::now();
                wynik_AStar = TSPAStar(graf, start, wierzcholkiAStar, wynikOptymalny);
                end_AStar = chrono::high_resolution_clock::now();

                for(int i = 0; i< graf.size()+1; i++){
                    cout << wierzcholkiAStar[i] << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki AStar: " << wynik_AStar << endl;

                czas_AStar = chrono::duration_cast<chrono::milliseconds>(end_AStar - start_AStar).count();
                cout << czas_AStar << " ms" << endl;

                //plik_wynikowy.open("results.txt");



//                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
//                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
//                plik_wynikowy << "BranchAndBound" << endl;                  // jaki algorytm
//                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
//                plik_wynikowy << wynik_BranchAndBound << endl;                // koszt sciezki
//                plik_wynikowy << czas_BranchAndBound  << endl;                // czas wykonania
//                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchAndBound[it] << " ";} plik_wynikowy << endl;// sciezka

                plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "BranchAndBound" << " "<< ite + 1 << " " << wynik_BranchAndBound << " " << czas_BranchAndBound << endl;
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchAndBound[it] << " ";} plik_wynikowy << endl;// sciezka

                plik_wynikowy_czasow << czesc_plikowa << " " << "BranchAndBound" << " " << czas_BranchAndBound << endl;

                plik_wynikowy_kosztow << czesc_plikowa << " " << "BranchAndBound" << " " << wynik_BranchAndBound << " " << wynikOptymalny << " " << blad << endl;

//                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
//                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
//                plik_wynikowy << "BestFirstSearch" << endl;                  // jaki algorytm
//                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
//                plik_wynikowy << wynik_BestFirstSearch << endl;                // koszt sciezki
//                plik_wynikowy << czas_BestFirstSearch  << endl;                // czas wykonania
//                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchAndBound[it] << " ";} plik_wynikowy << endl;// sciezka

                plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "BestFirstSearch" << " " << ite + 1 << " " << wynik_BestFirstSearch << " " << czas_BestFirstSearch << endl;
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBestFirstSearch[it] << " ";} plik_wynikowy << endl; // sciezka

                plik_wynikowy_czasow << czesc_plikowa << " " << "BestFirstSearch" << " " << czas_BestFirstSearch << endl;

                plik_wynikowy_kosztow << czesc_plikowa << " " << "BestFirstSearch" << " " << wynik_BestFirstSearch << " " << wynikOptymalny << " " << blad << endl;

//                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
//                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
//                plik_wynikowy << "AStar" << endl;                  // jaki algorytm
//                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
//                plik_wynikowy << wynik_AStar << endl;                // koszt sciezki
//                plik_wynikowy << czas_AStar  << endl;                // czas wykonania
//                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchAndBound[it] << " ";} plik_wynikowy << endl;// sciezka

                plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "AStar" << " " << ite + 1 << " " << wynik_AStar << " " << czas_AStar << endl;
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiAStar[it] << " ";} plik_wynikowy << endl; // sciezka

                plik_wynikowy_czasow << czesc_plikowa << " " << "AStar" << " " << czas_AStar << endl;

                plik_wynikowy_kosztow << czesc_plikowa << " " << "AStar" << " " << wynik_AStar << " " << wynikOptymalny << " " << blad << endl;

                //porownajZOptymalnym(wynik_BestFirstSearch, wynikOptymalny);

                plik_wynikowy << "" << endl;
                plik_wynikowy << "" << endl;
                plik_wynikowy << "Jak odczytywac plik wynikowy?" << endl;
                plik_wynikowy << "Nazwa pliku z grafem" << endl;
                plik_wynikowy << "Rozmiar grafu" << endl;
                plik_wynikowy << "Algorytm" << endl;
                plik_wynikowy << "Ktora iteracja" << endl;
                plik_wynikowy << "Koszt sciezki" << endl;
                plik_wynikowy << "Czas wykonania" << endl;
                plik_wynikowy << "Sciezka" << endl;

                plik_wynikowy.close();

            }

            break;
        default: /////////////////// TESTY TESTY TESTY TESTY

//            vector<string> pliki_z_danymi = {
//                    "asymetryczne_tsp17.txt", "asymetryczne_tsp33.txt", "asymetryczne_tsp64.txt",
//                    "identyczne_tsp5.txt", "identyczne_tsp6.txt", "identyczne_tsp7.txt", "identyczne_tsp8.txt",
//                    "identyczne_tsp9.txt", "identyczne_tsp10.txt", "niepelne_asymetryczne_tsp5.txt",
//                    "niepelne_asymetryczne_tsp6.txt", "niepelne_asymetryczne_tsp7.txt", "niepelne_asymetryczne_tsp8.txt",
//                    "niepelne_asymetryczne_tsp9.txt", "niepelne_asymetryczne_tsp10.txt", "niepelne_asymetryczne_tsp11.txt",
//                    "niepelne_asymetryczne_tsp12.txt", "niepelne_asymetryczne_tsp13.txt", "niepelne_asymetryczne_tsp14.txt",
//                    "niepelne_tsp5.txt", "niepelne_tsp6.txt", "niepelne_tsp7.txt", "niepelne_tsp8.txt", "niepelne_tsp9.txt",
//                    "niepelne_tsp10.txt", "niepelne_tsp11.txt", "niepelne_tsp12.txt", "tsp4.txt", "tsp5.txt",
//                    "tsp6.txt", "tsp7.txt", "tsp8.txt", "tsp9.txt", "tsp10.txt", "tsp14.txt", "tsp16.txt",
//                    "tsp17.txt", "tsp21.txt", "tsp22.txt", "tsp24.txt"
//            };

            vector<string> dzialajace_pliki_z_danymi = {
                    /*"asymetryczne_tsp17.txt", "asymetryczne_tsp33.txt", "asymetryczne_tsp64.txt",*/
                    "identyczne_tsp5.txt", "identyczne_tsp6.txt", "identyczne_tsp7.txt", "identyczne_tsp8.txt",
                    "identyczne_tsp9.txt", "identyczne_tsp10.txt", "niepelne_asymetryczne_tsp5.txt",
                    "niepelne_asymetryczne_tsp6.txt", "niepelne_asymetryczne_tsp7.txt", "niepelne_asymetryczne_tsp8.txt",
                    "niepelne_asymetryczne_tsp9.txt", "niepelne_asymetryczne_tsp10.txt", "niepelne_asymetryczne_tsp11.txt",
                    "niepelne_asymetryczne_tsp12.txt", "niepelne_asymetryczne_tsp13.txt", "niepelne_asymetryczne_tsp14.txt",
                    "tsp4.txt", "tsp5.txt",
                    "tsp6.txt", "tsp7.txt", "tsp8.txt", "tsp10.txt", "tsp14.txt", "tsp16.txt",
                    "tsp17.txt", "tsp21.txt"
            };

            for (auto& nazwa_pliku_1z40 : dzialajace_pliki_z_danymi) {
                string nazwa_pliku_bez_sciezki = nazwa_pliku_1z40.substr(nazwa_pliku_1z40.find_last_of("/\\") + 1);
                graf = parseTXT(nazwa_pliku_1z40, wynikOptymalny);
                int liczba_wierzcholkow = graf.size();

                int liczba_iteracji = 6;

                if (liczba_wierzcholkow <= 10) {
                    liczba_iteracji = 10;
                } else if (liczba_wierzcholkow <= 20) {
                    liczba_iteracji = 6;
                } else {
                    liczba_iteracji = 3;
                }


                    for (int ite = 0; ite < liczba_iteracji; ite++) {
                        cout << "Przetwarzanie pliku: " << nazwa_pliku_bez_sciezki << ", Iteracja: " << (ite + 1) << endl;

                        // -------------------- BranchAndBound --------------------
//                        vector<int> lepszeWierzcholkiBranchAndBound;
//                        auto lepszyStart_BranchAndBound = chrono::high_resolution_clock::now();
//                        double lepszyWynik_BranchAndBound = TSPBranchAndBound(graf, start, wierzcholkiBranchAndBound, wynikOptymalny);
//                        auto lepszyEnd_BranchAndBound = chrono::high_resolution_clock::now();
//                        auto lepszyCzas_BranchAndBound = chrono::duration_cast<chrono::milliseconds>(lepszyEnd_BranchAndBound - lepszyStart_BranchAndBound).count();
//                        plik_wynikowy << czesc_plikowa << " " << graf.size() << " " << "BranchAndBound" << " "<< ite + 1 << " " << wynik_BranchAndBound << " " << czas_BranchAndBound << endl;
//                        for (int iterac = 0; iterac < graf.size() + 1; iterac++) {plik_wynikowy << wierzcholkiBranchAndBound[iterac] << " ";} plik_wynikowy << endl;// sciezka
//                        //for (int iterac = 0; iterac < liczba_wierzcholkow; iterac++) {plik_wynikowy << lepszeWierzcholkiBranchAndBound[iterac] << " ";} plik_wynikowy << endl;// sciezka
//                        plik_wynikowy_czasow << czesc_plikowa << " " << "BranchAndBound" << " " << lepszyCzas_BranchAndBound << endl;
//                        plik_wynikowy_kosztow << czesc_plikowa << " " << "BranchAndBound" << " " << lepszyWynik_BranchAndBound << " " << wynikOptymalny << " " << blad << endl;

                        start_BranchAndBound = chrono::high_resolution_clock::now();
                        wynik_BranchAndBound = TSPBranchAndBound(graf, start, wierzcholkiBranchAndBound, wynikOptymalny);
                        end_BranchAndBound = chrono::high_resolution_clock::now();
                        czas_BranchAndBound = chrono::duration_cast<chrono::milliseconds>(end_BranchAndBound - start_BranchAndBound).count();
                        blad = ((wynik_BranchAndBound - wynikOptymalny)/wynik_BranchAndBound) * 100;

                        for(int i = 0; i< graf.size()+1; i++){
                            cout << wierzcholkiBranchAndBound[i] << " ";
                        }
                        cout << "" << endl;
                        cout << "Koszt optymalnej sciezki BNB: " << wynik_BranchAndBound << endl;

                        cout << czas_BranchAndBound << " ms" << endl;
                        plik_wynikowy << nazwa_pliku_bez_sciezki << " " << graf.size() << " " << "BranchAndBound" << " "<< ite + 1 << " " << wynik_BranchAndBound << " " << czas_BranchAndBound << endl;
                        for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchAndBound[it] << " ";} plik_wynikowy << endl;// sciezka
                        plik_wynikowy_czasow << nazwa_pliku_bez_sciezki << " " << "BranchAndBound" << " " << czas_BranchAndBound << endl;
                        plik_wynikowy_kosztow << nazwa_pliku_bez_sciezki << " " << "BranchAndBound" << " " << wynik_BranchAndBound << " " << wynikOptymalny << " " << blad << endl;

                    }

                    if(liczba_wierzcholkow <= 12){
                        for (int iter = 0; iter < liczba_iteracji; ++iter) {
                            //

                            cout << "Przetwarzanie pliku: " << nazwa_pliku_bez_sciezki << ", Iteracja: " << (iter + 1) << endl;
                            // -------------------- BestFirstSearch --------------------
                            vector<int> lepszeWierzcholkiBestFirstSearch;
                            auto lepszyStart_BestFirstSearch = chrono::high_resolution_clock::now();
                            double lepszyWynik_BestFirstSearch = TSPBestFirstSearch(graf, start, lepszeWierzcholkiBestFirstSearch, wynikOptymalny);
                            auto lepszyEnd_BestFirstSearch = chrono::high_resolution_clock::now();
                            auto lepszyCzas_BestFirstSearch = chrono::duration_cast<chrono::milliseconds>(lepszyEnd_BestFirstSearch - lepszyStart_BestFirstSearch).count();

                            plik_wynikowy << nazwa_pliku_bez_sciezki << graf.size() << "BestFirstSearch" << iter + 1 << lepszyWynik_BestFirstSearch << lepszyCzas_BestFirstSearch << endl;
                            for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << lepszeWierzcholkiBestFirstSearch[it] << " ";} plik_wynikowy << endl; // sciezka
                            plik_wynikowy_czasow << nazwa_pliku_bez_sciezki << " " << "BestFirstSearch" << " " << lepszyCzas_BestFirstSearch << endl;
                            plik_wynikowy_kosztow << nazwa_pliku_bez_sciezki << " " << "BestFirstSearch" << " " << lepszyWynik_BestFirstSearch << " " << wynikOptymalny << " " << blad << endl;

                        }
                    }

                for (int itera = 0; itera < liczba_iteracji; ++itera) {
                    cout << "Przetwarzanie pliku: " << nazwa_pliku_bez_sciezki << ", Iteracja: " << (itera + 1) << endl;
                    // -------------------- AStar --------------------
                    vector<int> lepszeWierzcholkiAStar;
                    auto lepszyStart_AStar = chrono::high_resolution_clock::now();
                    double lepszyWynik_AStar = TSPAStar(graf, start, lepszeWierzcholkiAStar,wynikOptymalny);
                    auto lepszyEnd_AStar = chrono::high_resolution_clock::now();
                    auto lepszyCzas_AStar = chrono::duration_cast<chrono::milliseconds>(lepszyEnd_AStar - lepszyStart_AStar).count();

                    plik_wynikowy << nazwa_pliku_bez_sciezki << graf.size() << "AStar" << itera + 1 << lepszyWynik_AStar << lepszyCzas_AStar << endl;
                    for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << lepszeWierzcholkiAStar[it] << " ";} plik_wynikowy << endl; // sciezka
                    plik_wynikowy_czasow << nazwa_pliku_bez_sciezki << " " << "AStar" << " " << lepszyCzas_AStar << endl;
                    plik_wynikowy_kosztow << nazwa_pliku_bez_sciezki << " " << "AStar" << " " << lepszyWynik_AStar << " " << wynikOptymalny << " " << blad << endl;

                }
            }

            plik_wynikowy << "" << endl;
            plik_wynikowy << "" << endl;
            plik_wynikowy << "Jak odczytywac plik wynikowy?" << endl;
            plik_wynikowy << "Nazwa pliku z grafem" << endl;
            plik_wynikowy << "Rozmiar grafu" << endl;
            plik_wynikowy << "Algorytm" << endl;
            plik_wynikowy << "Ktora iteracja" << endl;
            plik_wynikowy << "Koszt sciezki" << endl;
            plik_wynikowy << "Czas wykonania" << endl;
            plik_wynikowy << "Sciezka" << endl;

            plik_wynikowy.close(); // Zamknij plik po zakończeniu wszystkich obliczeń
            break;

    }

//    std::ifstream file("../Diagrams.py");
//    if (!file) {
//        std::cerr << "Plik Diagrams.py nie istnieje" << std::endl;
//        return 1; // Wyjdź z programu z kodem bledu
//    } else {
//        std::cout << "Plik Diagrams.py istnieje" << std::endl;
//    }
//
//    int result = std::system("python3 ../Diagrams.py");
//    if (result != 0) {
//        std::cerr << "Nie udalo sie stworzyc wykresow. Kod wyjscia: " << result << std::endl;
//    } else {
//        std::cout << "Wykresy stworzone pomyslnie" << std::endl;
//    }

    cin.get();
    return 0;
}




void readConfig(string& nazwa_pliku,int& iteracje,int& wybor_algorytmu){

    // UZYWAC DO PRACY W CLION
    ifstream configFile("../config.txt");

    // UZYWAC DO PRACY W CMD
    //ifstream configFile("config.txt");

    if (configFile.is_open()) {
        //cout << "Otwarto plik konfiguracyjny";
        getline(configFile, nazwa_pliku);       // nazwa pliku z danymi
        configFile >> wybor_algorytmu;          // wybor algorytmu
        configFile >> iteracje;                 // ilosć powtorzeń

        configFile.close();                     // zamykamy plik po odczycie
    } else {
        cerr << "Nie mozna otworzyc pliku konfiguracyjnego" << endl;
    }

}

/*string& nazwa_pliku*/
vector<vector<double>> parseTXT(string& nazwa_pliku, double& wynikOptymalny) {

    string czesc_sciezki = "../data/";
    nazwa_pliku = czesc_sciezki + nazwa_pliku;

    // UZYWAC TEGO DO CMD
//    cout << "Nazwa pliku to " << nazwa_pliku << endl;
//    if (nazwa_pliku.rfind("../", 0) == 0) { // Sprawdza, czy scieżka zaczyna sie od "../"
//        nazwa_pliku.erase(0, 3); // Usuwa pierwsze trzy znaki "../"
//    }
//    cout << "Poprawiona nazwa pliku to " << nazwa_pliku << endl;

    // ZWYKLA NAZWE PLIKU UZYWAC DO CLIONA
    ifstream plikTXT(nazwa_pliku);
    if (!plikTXT.is_open()) {
        cerr << "Nie mozna otworzyc pliku z grafem!" << endl;
    }

    // pierwsza linijka to rozmiar grafu
    int n;
    plikTXT >> n;

    //macierz n x n
    vector<vector<double>> macierz_sasiedztwa(n, vector<double>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            plikTXT >> macierz_sasiedztwa[i][j];
        }
    }

    plikTXT >> wynikOptymalny;

    plikTXT.close();

    return macierz_sasiedztwa;
}

double TSPBranchAndBound(std::vector<std::vector<double>> graf, int start, std::vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna) {
    int N = graf.size();
    std::vector<int> obecna_sciezka; // inicjalizujemy sciezke
    std::vector<bool> odwiedzone(N, false); // odwiedzone wierzcholki
    double final_res = std::numeric_limits<double>::max(); // najlepszy koszt
    //double final_res = TSPNearestNeighbor(graf, start, wierzcholki_sciezki_minimalnej, wartoscOptymalna);
    std::vector<int> najlepsza_sciezka; // sciezka koncowa

    // funkcja do obliczania oszacowania dolnego - najkrotszej krawedzi z nieodwiedzonych wierzcholkow
    // oblicza ograniczenie dolne dla pozostalych NIEODWIEDZONYCH wierzcholkow
    // zatem z kazda iteracja zmniejsza sie to ograniczenie dolne
    auto oszacujDolneOgraniczenie = [&](std::vector<bool>& odwiedzone) -> double {
        double koszt = 0;
        for (int i = 0; i < N; i++) {
            if (!odwiedzone[i]) {
                double minimalny_koszt = std::numeric_limits<double>::max();
                for (int j = 0; j < N; j++) {
                    if (graf[i][j] != 0 && i != j) {
                        minimalny_koszt = std::min(minimalny_koszt, graf[i][j]);
                    }
                }
                if (minimalny_koszt != std::numeric_limits<double>::max()) {
                    koszt += minimalny_koszt;
                }
            }
        }
        return koszt;
    };

    auto start_time = std::chrono::high_resolution_clock::now();

    // funkcja lambda branch and bound
    std::function<void(int, double)> TSPRec = [&](int obecny_wierzcholek, double obecny_koszt) {
        auto current_time = std::chrono::high_resolution_clock::now();
        double elapsed_time = std::chrono::duration<double>(current_time - start_time).count();
//        if (elapsed_time > 1800.0) {
//            return;
//        }

        // dodajemy wierzcholek do obecnej sciezki
        obecna_sciezka.push_back(obecny_wierzcholek);
        odwiedzone[obecny_wierzcholek] = true;

        // jesli odwiedzilismy wszystkie wierzcholki i wracamy do poczatkowego
        if (obecna_sciezka.size() == N) {
            if (graf[obecny_wierzcholek][start] != 0) {
                double pelny_koszt = obecny_koszt + graf[obecny_wierzcholek][start];
                if (pelny_koszt < final_res) {
                    final_res = pelny_koszt;
                    najlepsza_sciezka = obecna_sciezka;
                    najlepsza_sciezka.push_back(start);

                    if (final_res <= wartoscOptymalna) {
                        wierzcholki_sciezki_minimalnej = najlepsza_sciezka;
                        return;
                    }
                }
            }
        } else {
            // obliczamy dolne oszacowanie dla obecnej sciezki
            double dolne_oszacowanie = obecny_koszt + oszacujDolneOgraniczenie(odwiedzone);

            // sprawdzamy, czy dolne oszacowanie przekracza obecne najlepsze
            if (dolne_oszacowanie < final_res) {
                for (int i = 0; i < N; i++) {
                    if (!odwiedzone[i] && graf[obecny_wierzcholek][i] != 0) {
                        TSPRec(i, obecny_koszt + graf[obecny_wierzcholek][i]);

                        if (final_res <= wartoscOptymalna) {
                            return;
                        }
                    }
                }
            }
        }

        // usuwamy wierzcholek z obecnej sciezki i cofamy sie
        odwiedzone[obecny_wierzcholek] = false;
        obecna_sciezka.pop_back();
    };

    // od wierzcholka startowego
    TSPRec(start, 0);

    if (!najlepsza_sciezka.empty()) {
        wierzcholki_sciezki_minimalnej = najlepsza_sciezka;
    }

    return final_res;
}

double TSPBestFirstSearch(vector<vector<double>>& graf, int start, vector<int>& sciezka_minimalna, double wartoscOptymalna) {
    int n = graf.size();
    // maska funkcji oceny
    vector<vector<double>> gWynik(1 << n,
                                  vector<double>(n, numeric_limits<double>::infinity())
    );
    // odtwarzanie sciezki
    vector<vector<int>> poprzednik(1 << n,
                                   vector<int>(n, -1)
    );

    // kolejka priorytetowa — sortujemy po dotychczasowym koszcie g
    auto porownanie = [](auto &lhs, auto &rhs) {
        return lhs.first > rhs.first;
    };
    priority_queue<
            pair<double, pair<int,int>>,
            vector<pair<double, pair<int,int>>>,
            decltype(porownanie)
    > zbiorOtwarte(porownanie);

    // wierzcholek poczatkowy nie przechodzi w samego siebie
    gWynik[1 << start][start] = 0.0;
    zbiorOtwarte.push({0.0, {1 << start, start}});


    double najlepszyKoszt = numeric_limits<double>::infinity();
    vector<int> najlepszaSciezka;

    while (!zbiorOtwarte.empty()) {
        auto [kosztObecny, stan] = zbiorOtwarte.top();
        zbiorOtwarte.pop();

        // jesli aktualny koszt jest już wiekszy niż dotad znaleziony najlepszy,
        // to dalsze przegladanie stanow z kolejki nie ma sensu
        if (kosztObecny >= najlepszyKoszt) {
            break;
        }

        int maska = stan.first;
        int wierzcholek = stan.second;

        // jesli maska = (1<<n)-1, to znaczy że odwiedzilismy wszystkie wierzcholki
        // teraz probujemy wrocić do startu, o ile krawedź istnieje (graf[wierzcholek][start] > 0)
        if (maska == (1 << n) - 1) {
            if (graf[wierzcholek][start] > 0) {
                double kosztCalkowity = gWynik[maska][wierzcholek]
                                        + graf[wierzcholek][start];
                if (kosztCalkowity < najlepszyKoszt) {
                    najlepszyKoszt = kosztCalkowity;
                    // Odtwarzamy scieżke
                    vector<int> sciezka;
                    int curr = wierzcholek;
                    int currMask = maska;
                    while (curr != -1) {
                        sciezka.push_back(curr);
                        int prev = poprzednik[currMask][curr];
                        // Wyzeruj bit odwiedzenia, żeby cofnać sie w masce
                        currMask &= ~(1 << curr);
                        curr = prev;
                    }
                    reverse(sciezka.begin(), sciezka.end());
                    sciezka.push_back(start);

                    najlepszaSciezka = sciezka;
                }
            }
            // kontynuujemy przeglad, bo być może istnieje jeszcze inna scieżka
            // o niższym koszcie w kolejce
            continue;
        }

        // rozwijamy sasiadow
        for (int sasiad = 0; sasiad < n; ++sasiad) {
            // sasiad nie byl juz odwiedzony oraz czy istnieje krawedz
            if (((maska & (1 << sasiad)) == 0) && (graf[wierzcholek][sasiad] > 0)) {
                int nowaMaska = maska | (1 << sasiad);
                double nowyKoszt = gWynik[maska][wierzcholek] + graf[wierzcholek][sasiad];

                if (nowyKoszt < gWynik[nowaMaska][sasiad]) {
                    gWynik[nowaMaska][sasiad] = nowyKoszt;
                    poprzednik[nowaMaska][sasiad] = wierzcholek;
                    zbiorOtwarte.push({nowyKoszt, {nowaMaska, sasiad}});
                }
            }
        }
    }

    // zwracamy najlepszyKoszt i scieżke
    if (najlepszyKoszt < numeric_limits<double>::infinity()) {
        sciezka_minimalna = najlepszaSciezka;
        return najlepszyKoszt;
    } else {
        // gdy graf nie jest pelny
        sciezka_minimalna.clear();
        return numeric_limits<double>::infinity();
    }
}

double TSPAStar(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna) {
    int n = graf.size();

    // Koszt dotarcia do (maska, wierzcholek)
    vector<vector<double>> gWynik(1 << n,
                                  vector<double>(n, numeric_limits<double>::infinity()));

    // fWynik = g + h
    vector<vector<double>> fWynik(1 << n,
                                  vector<double>(n, numeric_limits<double>::infinity()));

    // Poprzednik do odtwarzania scieżki
    vector<vector<int>> poprzednik(1 << n,
                                   vector<int>(n, -1));

    // Heurystyka: najkrotsza krawedź wychodzaca z wierzcholka w
    // (z pominieciem krawedzi 0, bo to "brak krawedzi")
    auto heurystyka = [&](int wierzcholek) {
        double min_edge = numeric_limits<double>::infinity();
        for (int i = 0; i < n; i++) {
            if (i != wierzcholek && graf[wierzcholek][i] > 0) {
                min_edge = min(min_edge, graf[wierzcholek][i]);
            }
        }
        // Jesli nie znaleziono żadnej dodatniej krawedzi, heurystyka = ∞
        // (bo wierzcholek jest „odciety” – nigdzie nie można przejsć)
        return min_edge;
    };

    // Porownujemy stany w kolejce po najmniejszym f = g + h
    auto porownanie = [](auto &lhs, auto &rhs) {
        return lhs.first > rhs.first;
    };
    priority_queue<
            pair<double, pair<int,int>>,
            vector<pair<double, pair<int,int>>>,
            decltype(porownanie)
    > zbiorOtwarte(porownanie);

    // Inicjalizacja dla stanu (maska = 1 << start, wierzcholek = start)
    gWynik[1 << start][start] = 0.0;
    fWynik[1 << start][start] = 0.0 + heurystyka(start);
    zbiorOtwarte.push({fWynik[1 << start][start], {1 << start, start}});

    // Najlepszy koszt (aktualizujemy, jesli znajdziemy pelny cykl)
    double najlepszyKoszt = numeric_limits<double>::infinity();
    // Aby zapamietać najlepsza scieżke
    vector<int> najlepszaSciezka;

    while (!zbiorOtwarte.empty()) {
        // Wyciagamy stan o najmniejszym f = g + h
        auto [fAktualne, stan] = zbiorOtwarte.top();
        zbiorOtwarte.pop();

        int maska = stan.first;
        int wierzcholek = stan.second;
        double gAktualne = gWynik[maska][wierzcholek]; // nasz bieżacy koszt dotarcia

        // Jesli fAktualne >= najlepszyKoszt, to dalsze szukanie
        // nie poprawi już wyniku (A* – monotoniczne).
        if (fAktualne >= najlepszyKoszt) {
            break;
        }

        // Czy odwiedzilismy wszystkie wierzcholki?
        if (maska == (1 << n) - 1) {
            // Możemy domknać cykl, jesli krawedź (wierzcholek -> start) istnieje
            if (graf[wierzcholek][start] > 0) {
                double kosztCalkowity = gAktualne + graf[wierzcholek][start];
                if (kosztCalkowity < najlepszyKoszt) {
                    najlepszyKoszt = kosztCalkowity;

                    // Odtwarzamy scieżke
                    vector<int> sciezka;
                    int curr = wierzcholek;
                    int currMask = maska;
                    while (curr != -1) {
                        sciezka.push_back(curr);
                        int prev = poprzednik[currMask][curr];
                        currMask &= ~(1 << curr);
                        curr = prev;
                    }
                    reverse(sciezka.begin(), sciezka.end());
                    // Domkniecie
                    sciezka.push_back(start);

                    najlepszaSciezka = sciezka;
                }
            }
            // Kontynuuj, bo w kolejce moga być stany z jeszcze mniejszym f
            continue;
        }

        // Rozwijamy sasiadow (wierzcholkow) – tych nieodwiedzonych
        for (int sasiad = 0; sasiad < n; ++sasiad) {
            // Warunek: bit sasiada w masce == 0 (nieodwiedzony) i krawedź istnieje ( > 0)
            if (((maska & (1 << sasiad)) == 0) && (graf[wierzcholek][sasiad] > 0)) {
                int nowaMaska = maska | (1 << sasiad);
                double gNowe = gAktualne + graf[wierzcholek][sasiad];

                if (gNowe < gWynik[nowaMaska][sasiad]) {
                    gWynik[nowaMaska][sasiad] = gNowe;
                    poprzednik[nowaMaska][sasiad] = wierzcholek;

                    // f = g + h
                    double h = heurystyka(sasiad);
                    double fNowe = gNowe + h;

                    fWynik[nowaMaska][sasiad] = fNowe;
                    zbiorOtwarte.push({fNowe, {nowaMaska, sasiad}});
                }
            }
        }
    }

    // Zwracamy najlepsza dotad znaleziona trase (i koszt)
    if (najlepszyKoszt < numeric_limits<double>::infinity()) {
        wierzcholki_sciezki_minimalnej = najlepszaSciezka;
        return najlepszyKoszt;
    } else {
        // Nie ma pelnej trasy Hamiltona (z powrotem do startu)
        wierzcholki_sciezki_minimalnej.clear();
        return numeric_limits<double>::infinity();
    }
}

double TSPNearestNeighbor(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna) {
    vector<int> bufor_wierzcholkow;
    vector<bool> odwiedzone(graf.size(), false);

    odwiedzone[start] = true;
    int obecny_wierzcholek = start;
    double koszt = 0;

    wierzcholki_sciezki_minimalnej.clear();
    wierzcholki_sciezki_minimalnej.push_back(start);

    auto czas_poczatkowy = std::chrono::high_resolution_clock::now();

    for (int i = 1; i < graf.size(); i++) {
        auto obecny_czas = std::chrono::high_resolution_clock::now();
        auto miniony_czas = std::chrono::duration_cast<std::chrono::minutes>(obecny_czas - czas_poczatkowy);


        //koszt = 0;
        int nastepny_wierzcholek = -1;
        double minimalna_sciezka = INT_MAX;

        for (int j = 0; j < graf.size(); j++) { //szukamy najblizszego wierzcholka wzgledem wierzcholka z indeksem i
            if (!odwiedzone[j] && graf[obecny_wierzcholek][j] < minimalna_sciezka) { // jesli wierzcholek nie zostal odwiedzony i znaleziono lepsza krawedz
                minimalna_sciezka = graf[obecny_wierzcholek][j]; // aktualizujemy dystans
                //bufor_wierzcholkow.push_back(j); // dodajemy wierzcholek do bufora
                nastepny_wierzcholek = j;
            }
        }

        if (nastepny_wierzcholek == -1 || minimalna_sciezka == INT_MAX) {
            cout << "Rozwiazanie nie istnieje, brak krawedzi do nieodwiedzonego wierzcholka" << endl;
            return 0;
        }

        odwiedzone[nastepny_wierzcholek] = true;
        koszt += minimalna_sciezka;

//        bufor_wierzcholkow.push_back(start);
//        obecny_wierzcholek = nastepny_wierzcholek;
//        wierzcholki_sciezki_minimalnej = bufor_wierzcholkow;
        wierzcholki_sciezki_minimalnej.push_back(nastepny_wierzcholek);
        obecny_wierzcholek = nastepny_wierzcholek;

        if (miniony_czas.count() >= 30) {
            cout << "Czas wykonania przekroczyl 30 minut. Program zakonczony." << endl;
            return koszt;
        }
    }

    if(koszt == wartoscOptymalna && wartoscOptymalna != -2){
        wierzcholki_sciezki_minimalnej = bufor_wierzcholkow;
        for (int k = 0; k < graf.size() + 1; k++){ // + 1 bo wracamy do wierzcholka poczatkowego
       //     cout << wierzcholki_sciezki_minimalnej[k] << " ";
        }
        return wartoscOptymalna;
    }

    // iterujemy przez kazdy istniejacy wierzcholek jaka sciezka
    // np: od wierzcholka startowego 0 przechodzimy do wierzcholka 1, nie sprawdzamy krawedzi 01 bo już jest odwiedzona
    // zatem od wierzcholka 1 sprawdzamy jego dystans z wierzcholkiem 2. Wierzcholek 2 nie byl odwiedzany i krawedz 01 jest mniejsza niz nieskonczonosc.
    // minimalny dystans od wierzcholka 1 do innego staje sie dlugoscia krawedzi 12.

    if(graf[obecny_wierzcholek][start] != 0){
        koszt += graf[obecny_wierzcholek][start];
        wierzcholki_sciezki_minimalnej.push_back(start);
    } else {
        cout << "Rozwiazanie nie istnieje, brak sciezki powrotnej" << endl;
        return 0;
    }

//    for (int k = 0; k < graf.size() + 1; k++){ // + 1 bo wracamy do wierzcholka poczatkowego
//        cout << wierzcholki_sciezki_minimalnej[k] << " ";
//    }
//    cout << "" << endl;

    return koszt;
}

