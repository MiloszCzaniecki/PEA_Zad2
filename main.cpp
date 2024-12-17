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
double TSPBranchNBound(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna);
double TSPBestFirstSearch(vector<vector<double>>& graf, int start, vector<int>& sciezka_minimalna, double wartoscOptymalna);
double TSPAStar(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna);

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
    vector<int> wierzcholkiBranchNBound;
    vector<int> wierzcholkiBestFirstSearch;

    double wynik_BranchNBound = 0;
    double wynik_BestFirstSearch = 0;
    double wynik_AStar = 0;

    auto start_BranchNBound = chrono::high_resolution_clock::now();
    auto end_BranchNBound = chrono::high_resolution_clock::now();
    auto czas_BranchNBound = chrono::duration_cast<chrono::milliseconds>(end_BranchNBound - start_BranchNBound).count();

    auto start_BestFirstSearch = chrono::high_resolution_clock::now();
    auto end_BestFirstSearch = chrono::high_resolution_clock::now();
    auto czas_BestFirstSearch = chrono::duration_cast<chrono::milliseconds>(end_BestFirstSearch - start_BestFirstSearch).count();

    auto start_AStar = chrono::high_resolution_clock::now();
    auto end_AStar = chrono::high_resolution_clock::now();
    auto czas_AStar = chrono::duration_cast<chrono::milliseconds>(end_AStar - start_AStar).count();

    ofstream plik_wynikowy("results.txt");

    switch (wybor_algorytmu) {
        case 1: // BranchAndBound
            for(int ite =0; ite < iteracje; ite++){
                start_BranchNBound = chrono::high_resolution_clock::now();
                wynik_BranchNBound = TSPBranchNBound(graf, start, wierzcholkiBranchNBound, wynikOptymalny);
                end_BranchNBound = chrono::high_resolution_clock::now();

                for(int i = 0; i< graf.size()+1; i++){
                    cout << wierzcholkiBranchNBound[i] << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki BNB: " << wynik_BranchNBound << endl;

                czas_BranchNBound = chrono::duration_cast<chrono::milliseconds>(end_BranchNBound - start_BranchNBound).count();
                cout << czas_BranchNBound << " ms" << endl;


                //plik_wynikowy.open("results.txt");

                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
                plik_wynikowy << "BranchAndBound" << endl;                  // jaki algorytm
                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
                plik_wynikowy << wynik_BranchNBound << endl;                // koszt sciezki
                plik_wynikowy << czas_BranchNBound  << endl;                // czas wykonania
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchNBound[it] << " ";} // sciezka




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

                //plik_wynikowy.open("results.txt");

                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
                plik_wynikowy << "BestFirstSearch" << endl;                  // jaki algorytm
                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
                plik_wynikowy << wynik_BestFirstSearch << endl;                // koszt sciezki
                plik_wynikowy << czas_BestFirstSearch  << endl;                // czas wykonania
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBestFirstSearch[it] << " ";} // sciezka




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

                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
                plik_wynikowy << "AStar" << endl;                  // jaki algorytm
                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
                plik_wynikowy << wynik_AStar << endl;                // koszt sciezki
                plik_wynikowy << czas_AStar  << endl;                // czas wykonania
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiAStar[it] << " ";} // sciezka




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
                start_BranchNBound = chrono::high_resolution_clock::now();
                wynik_BranchNBound = TSPBranchNBound(graf, start, wierzcholkiBranchNBound, wynikOptymalny);
                end_BranchNBound = chrono::high_resolution_clock::now();

                for(int i = 0; i< graf.size()+1; i++){
                    cout << wierzcholkiBranchNBound[i] << " ";
                }
                cout << "" << endl;
                cout << "Koszt optymalnej sciezki BNB: " << wynik_BranchNBound << endl;

                czas_BranchNBound = chrono::duration_cast<chrono::milliseconds>(end_BranchNBound - start_BranchNBound).count();
                cout << czas_BranchNBound << " ms" << endl;

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

                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
                plik_wynikowy << "BranchAndBound" << endl;                  // jaki algorytm
                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
                plik_wynikowy << wynik_BranchNBound << endl;                // koszt sciezki
                plik_wynikowy << czas_BranchNBound  << endl;                // czas wykonania
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchNBound[it] << " ";} // sciezka

                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
                plik_wynikowy << "BestFirstSearch" << endl;                  // jaki algorytm
                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
                plik_wynikowy << wynik_BestFirstSearch << endl;                // koszt sciezki
                plik_wynikowy << czas_BestFirstSearch  << endl;                // czas wykonania
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchNBound[it] << " ";} // sciezka

                plik_wynikowy << nazwa_pliku << endl;                       // plik z grafem
                plik_wynikowy << graf.size() << endl;                       // rozmiar grafu
                plik_wynikowy << "AStar" << endl;                  // jaki algorytm
                plik_wynikowy << ite + 1<< endl;                               // ktora iteracja
                plik_wynikowy << wynik_AStar << endl;                // koszt sciezki
                plik_wynikowy << czas_AStar  << endl;                // czas wykonania
                for (int it = 0; it < graf.size() + 1; it++) {plik_wynikowy << wierzcholkiBranchNBound[it] << " ";} // sciezka

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

            vector<string> pliki_z_danymi = {
                    "asymetryczne_tsp17.txt", "asymetryczne_tsp33.txt", "asymetryczne_tsp64.txt",
                    "identyczne_tsp5.txt", "identyczne_tsp6.txt", "identyczne_tsp7.txt", "identyczne_tsp8.txt",
                    "identyczne_tsp9.txt", "identyczne_tsp10.txt", "niepelne_asymetryczne_tsp5.txt",
                    "niepelne_asymetryczne_tsp6.txt", "niepelne_asymetryczne_tsp7.txt", "niepelne_asymetryczne_tsp8.txt",
                    "niepelne_asymetryczne_tsp9.txt", "niepelne_asymetryczne_tsp10.txt", "niepelne_asymetryczne_tsp11.txt",
                    "niepelne_asymetryczne_tsp12.txt", "niepelne_asymetryczne_tsp13.txt", "niepelne_asymetryczne_tsp14.txt",
                    "niepelne_tsp5.txt", "niepelne_tsp6.txt", "niepelne_tsp7.txt", "niepelne_tsp8.txt", "niepelne_tsp9.txt",
                    "niepelne_tsp10.txt", "niepelne_tsp11.txt", "niepelne_tsp12.txt", "tsp4.txt", "tsp5.txt",
                    "tsp6.txt", "tsp7.txt", "tsp8.txt", "tsp9.txt", "tsp10.txt", "tsp14.txt", "tsp16.txt",
                    "tsp17.txt", "tsp21.txt", "tsp22.txt", "tsp24.txt"
            };

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

                int liczba_iteracji;

                if (liczba_wierzcholkow <= 10) {
                    liczba_iteracji = 1;
                } else if (liczba_wierzcholkow <= 20) {
                    liczba_iteracji = 1;
                } else {
                    liczba_iteracji = 1;
                }


                    for (int ite = 0; ite < liczba_iteracji; ++ite) {
                        cout << "Przetwarzanie pliku: " << nazwa_pliku_bez_sciezki << ", Iteracja: " << (ite + 1) << endl;

                        // -------------------- BranchNBound --------------------
                        vector<int> lepszeWierzcholkiBranchNBound;
                        auto lepszyStart_BranchNBound = chrono::high_resolution_clock::now();
                        double lepszyWynik_BranchNBound = TSPBranchNBound(graf, start, lepszeWierzcholkiBranchNBound, wynikOptymalny);
                        auto lepszyEnd_BranchNBound = chrono::high_resolution_clock::now();
                        auto lepszyCzas_BranchNBound = chrono::duration_cast<chrono::milliseconds>(lepszyEnd_BranchNBound - lepszyStart_BranchNBound).count();

                        // Zapis do pliku wynikowego
                        plik_wynikowy << nazwa_pliku_bez_sciezki << endl;                // plik z grafem
                        plik_wynikowy << liczba_wierzcholkow << endl;        // rozmiar grafu
                        plik_wynikowy << "BranchNBound" << endl;           // jaki algorytm
                        //plik_wynikowy << ite + 1 << endl;                    // która iteracja
                        //plik_wynikowy << lepszyWynik_BranchNBound << endl;         // koszt ścieżki
                        plik_wynikowy << lepszyCzas_BranchNBound << " ms" << endl; // czas wykonania
                        for (int w : lepszeWierzcholkiBranchNBound) {
                            plik_wynikowy << w << " ";
                        }
                        plik_wynikowy << endl;
                    }

                    if(liczba_wierzcholkow <= 12){
                        for (int iter = 0; iter < liczba_iteracji; ++iter) {
                            cout << "Przetwarzanie pliku: " << nazwa_pliku_bez_sciezki << ", Iteracja: " << (iter + 1) << endl;
                            // -------------------- BestFirstSearch --------------------
                            vector<int> lepszeWierzcholkiBestFirstSearch;
                            auto lepszyStart_BestFirstSearch = chrono::high_resolution_clock::now();
                            double lepszyWynik_BestFirstSearch = TSPBestFirstSearch(graf, start, lepszeWierzcholkiBestFirstSearch, wynikOptymalny);
                            auto lepszyEnd_BestFirstSearch = chrono::high_resolution_clock::now();
                            auto lepszyCzas_BestFirstSearch = chrono::duration_cast<chrono::milliseconds>(
                                    lepszyEnd_BestFirstSearch - lepszyStart_BestFirstSearch).count();

                            // Zapis do pliku wynikowego
                            plik_wynikowy << nazwa_pliku_bez_sciezki << endl;                // plik z grafem
                            plik_wynikowy << liczba_wierzcholkow << endl;        // rozmiar grafu
                            plik_wynikowy << "BestFirstSearch" << endl;          // jaki algorytm
                            //plik_wynikowy << iter + 1 << endl;                    // która iteracja
                            //plik_wynikowy << lepszyWynik_BestFirstSearch << endl;      // koszt ścieżki
                            plik_wynikowy << lepszyCzas_BestFirstSearch << " ms" << endl; // czas wykonania
                            for (int w: lepszeWierzcholkiBestFirstSearch) {
                                plik_wynikowy << w << " ";
                            }
                            plik_wynikowy << endl;
                        }
                    }





                for (int itera = 0; itera < liczba_iteracji; ++itera) {
                    cout << "Przetwarzanie pliku: " << nazwa_pliku_bez_sciezki << ", Iteracja: " << (itera + 1) << endl;
                    // -------------------- AStar --------------------
                    vector<int> lepszeWierzcholkiAStar;
                    auto lepszyStart_AStar = chrono::high_resolution_clock::now();
                    double lepszyWynik_AStar = TSPAStar(graf, start, lepszeWierzcholkiAStar,
                                                                      wynikOptymalny);
                    auto lepszyEnd_AStar = chrono::high_resolution_clock::now();
                    auto lepszyCzas_AStar = chrono::duration_cast<chrono::milliseconds>(
                            lepszyEnd_AStar - lepszyStart_AStar).count();

                    // Zapis do pliku wynikowego
                    plik_wynikowy << nazwa_pliku_bez_sciezki << endl;                // plik z grafem
                    plik_wynikowy << liczba_wierzcholkow << endl;        // rozmiar grafu
                    plik_wynikowy << "AStar" << endl;                    // jaki algorytm
                    //plik_wynikowy << itera + 1 << endl;                    // która iteracja
                    //plik_wynikowy << lepszyWynik_AStar << endl;                // koszt ścieżki
                    plik_wynikowy << lepszyCzas_AStar << " ms" << endl;        // czas wykonania
                    for (int w: lepszeWierzcholkiAStar) {
                        plik_wynikowy << w << " ";
                    }
                    plik_wynikowy << endl;
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
        configFile >> wybor_algorytmu;          // wybór algorytmu
        configFile >> iteracje;                 // ilość powtórzeń

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
//    if (nazwa_pliku.rfind("../", 0) == 0) { // Sprawdza, czy ścieżka zaczyna się od "../"
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

double TSPBranchNBound(std::vector<std::vector<double>> graf, int start, std::vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna) {
    int N = graf.size();
    std::vector<int> obecna_sciezka; // inicjalizujemy sciezke
    std::vector<bool> odwiedzone(N, false); // odwiedzone wierzcholki
    double final_res = std::numeric_limits<double>::max(); // najlepszy koszt
    std::vector<int> najlepsza_sciezka; // sciezka koncowa

    // funkcja do obliczania oszacowania dolnego
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
        if (elapsed_time > 1800.0) {
            return;
        }

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

// niepoprawny BNB
double OldTSPBranchNBound(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna)
{
    int N = graf.size();
    int sciezka_koncowa[N + 1]; // sciezka koncowa
    bool odwiedzone[N];       // odwiedzone wierzcholki
    double final_res = std::numeric_limits<double>::max(); // najlepszy koszt

    int obecna_sciezka[N + 1]; // inicjalizujemy sciezke
    // obliczamy poczatkowe ograniczenie dolne dla korzenia
    // wzorem 1/2 * (pierwszy + drugi minimalny) dla każdego wierzchołka
    double obecne_ograniczenie = 0;
    memset(obecna_sciezka, -1, sizeof(obecna_sciezka));
    memset(odwiedzone, 0, sizeof(odwiedzone));

    // krawedz o najmniejszym koszcie dochodzaca do wierzcholka i
    auto firstMin = [&](const vector<vector<double>>& graf, int i) -> double {
        double min = std::numeric_limits<double>::max();
        for (int k = 0; k < N; k++)
            if (graf[i][k] < min && i != k)
                min = graf[i][k];
        return min;
    };

    // krawedz o drugim najmniejszym koszcie dochodzaca do wierzcholka i
    auto secondMin = [&](const vector<vector<double>>& graf, int i) -> double {
        double first = std::numeric_limits<double>::max();
        double second = std::numeric_limits<double>::max();
        for (int j = 0; j < N; j++) {
            if (i == j)
                continue;

            if (graf[i][j] <= first) {
                second = first;
                first = graf[i][j];
            } else if (graf[i][j] <= second && graf[i][j] != first)
                second = graf[i][j];
        }
        return second;
    };

    // obliczamy początkowe ograniczenie dolne
    for (int i = 0; i < N; i++)
        obecne_ograniczenie += (firstMin(graf, i) + secondMin(graf, i));
    obecne_ograniczenie =  obecne_ograniczenie / 2;

    odwiedzone[0] = true;
    obecna_sciezka[0] = 0;

    // Ustawienie początkowego czasu
    auto start_time = std::chrono::high_resolution_clock::now();

    // obecne_ograniczenie -> lower bound
    // curr_weight-> dotychczasowa waga sciezki
    // level-> obecny poziom podczas przeszukiwania
    // space tree
    // obecna_sciezka[] -> sciezka zawierajaca obecnie analizowane rozwiazanie sciezka_koncowa[]

    std::function<void(double, int, int, int*)> TSPRec = [&](double obecne_ograniczenie, double curr_weight, int level, int* obecna_sciezka) {
        // Sprawdzenie czasu działania
        auto current_time = std::chrono::high_resolution_clock::now();
        double elapsed_time = std::chrono::duration<double>(current_time - start_time).count();
        if (elapsed_time > 1800.0) {
            // Przekroczono czas działania - przerywamy rekursję
            cout << "Przekroczono czas dialania" << endl;
            return;
        }

        // jesli odwiedzono juz kazdy wierzcholek
        if (level == N) {
            // sprawdz czy istnieje krawedz od ostatniego wierzcholka do pierwszego
            if (graf[obecna_sciezka[level - 1]][obecna_sciezka[0]] != 0) {
                // curr_res to calkowita waga obecnie rozwazanego rozwiazania
                double curr_res = curr_weight + graf[obecna_sciezka[level - 1]][obecna_sciezka[0]];

                // jesli obecne rozwiazanie lepsze niz najlepsze -> przypisujemy
                if (curr_res < final_res) {
                    for (int i = 0; i < N; i++) {
                        sciezka_koncowa[i] = obecna_sciezka[i];
                    }
                    sciezka_koncowa[N] = obecna_sciezka[0];
                    final_res = curr_res;
                }
            }
            return;
        }

        // przez inne poziomy buduj rekursywnie przestrzeń do przeszukania
        for (int i = 0; i < N; i++) {
            // jesli wierzcholek jest nieodwiedzony i nie przechodzimy w samego siebie
            if (graf[obecna_sciezka[level - 1]][i] != 0 && odwiedzone[i] == false) {
                int tymczasowe_ograniczenie = obecne_ograniczenie;
                curr_weight += graf[obecna_sciezka[level - 1]][i];

                if (level == 1)
                    obecne_ograniczenie -= ((firstMin(graf, obecna_sciezka[level - 1]) + firstMin(graf, i)) / 2);
                else
                    obecne_ograniczenie -= ((secondMin(graf, obecna_sciezka[level - 1]) + firstMin(graf, i)) / 2);

                if (obecne_ograniczenie + curr_weight < final_res) {
                    obecna_sciezka[level] = i;
                    odwiedzone[i] = true;
                    // zwiedzamy kolejny poziom
                    TSPRec(obecne_ograniczenie, curr_weight, level + 1, obecna_sciezka);
                    odwiedzone[i] = false; // tego mozliwie brakowalo
                }

                // w innym przypadku obcinamy gałąź -
                // odejmujemy rozważaną wcześniej krawędź
                // i  nigdy nie wracamy
                curr_weight -= graf[obecna_sciezka[level - 1]][i]; // koszt trasy od poczatku do obecnego wierzcholka
                obecne_ograniczenie = tymczasowe_ograniczenie;

                // odnawiamy tablice
//                memset(odwiedzone, false, sizeof(odwiedzone));
//                for (int j = 0; j <= level - 1; j++)
//                    odwiedzone[obecna_sciezka[j]] = true;
//                odwiedzone[i] = false;

            }
        }
    };

    // wywołujemy lambdę TSPRec i zaczynamy wlasciwe dzialanie algorytmu
    TSPRec(obecne_ograniczenie, 0, 1, obecna_sciezka);

    // kopiujemy ze sciezka_koncowa do wierzcholki_sciezki_minimalnej
    std::copy(sciezka_koncowa, sciezka_koncowa + N + 1, std::back_inserter(wierzcholki_sciezki_minimalnej));

    return final_res;
}



double TSPBestFirstSearch(vector<vector<double>>& graf, int start, vector<int>& sciezka_minimalna, double wartoscOptymalna) {
    int n = graf.size();
    vector<bool> odwiedzone(n, false);
    vector<int> aktualna_sciezka;
    double minimalny_koszt = INF;

    // zapisanie czasu początkowego
    auto czas_poczatkowy = chrono::high_resolution_clock::now();

    // lambda funkcja do rekurencyjnego przeszukiwania sciezek
    auto tspBacktrack = [&](auto& tspBacktrack, int obecny_wierzcholek, double aktualny_koszt, int ileOdwiedzonych) -> void {
        // Sprawdzenie czasu działania
        auto obecny_czas = chrono::high_resolution_clock::now();
        double miniony_czas = chrono::duration<double>(obecny_czas - czas_poczatkowy).count();
        if (miniony_czas > 1800.0) {
            // Przekroczono czas działania - przerywamy rekursję
            cout << "Przekroczono 30 minut" << endl;
            return;
        }

        // jesli odwiedzilismy wszystkie wierzcholki, wroc do startu i sprawdz koszt
        if (ileOdwiedzonych == n) {
            if (graf[obecny_wierzcholek][start] != INF) {
                double calkowity_koszt = aktualny_koszt + graf[obecny_wierzcholek][start];
                if (calkowity_koszt < minimalny_koszt) {
                    minimalny_koszt = calkowity_koszt;
                    sciezka_minimalna = aktualna_sciezka;
                    sciezka_minimalna.push_back(start); // wracamy do punktu startowego
                }
            }
            return;
        }

        // przegladanie wszystkich sasiadow
        for (int i = 0; i < n; i++) {
            if (!odwiedzone[i] && graf[obecny_wierzcholek][i] != INF) {
                // oznacz wierzcholek jako odwiedzony
                odwiedzone[i] = true;
                aktualna_sciezka.push_back(i);

                // rekurencyjnie wywolaj dla kolejnego wierzcholka
                tspBacktrack(tspBacktrack, i, aktualny_koszt + graf[obecny_wierzcholek][i], ileOdwiedzonych + 1);

                // cofnij sie do poprzedniego stanu
                odwiedzone[i] = false;
                aktualna_sciezka.pop_back();
            }
        }
    };

    // inicjalizuj algorytm od wierzcholka startowego
    odwiedzone[start] = true;
    aktualna_sciezka.push_back(start);

    // uruchom funkcje rekurencyjna
    tspBacktrack(tspBacktrack, start, 0.0, 1);

    return minimalny_koszt;
}

double TSPAStar(vector<vector<double>> graf, int start, vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna) {
    int n = graf.size();
    vector<vector<double>> gWynik(1 << n, vector<double>(n, numeric_limits<double>::infinity()));
    vector<vector<double>> fWynik(1 << n, vector<double>(n, numeric_limits<double>::infinity()));
    vector<vector<int>> poprzednik(1 << n, vector<int>(n, -1));

    auto heurystyka = [&](const vector<vector<double>>& graf, int wierzcholek) {
        // heurystyka = najkrotsza krawedz wychodzaca z wierzcholka
        double min_edge = numeric_limits<double>::infinity();
        for (int i = 0; i < graf.size(); i++) {
            if (i != wierzcholek && graf[wierzcholek][i] < min_edge) {
                min_edge = graf[wierzcholek][i];
            }
        }
        return min_edge;
    };

    auto porownanie = [](const pair<double, pair<int, int>>& left, const pair<double, pair<int, int>>& right) {
        return left.first > right.first;
    };

    priority_queue<pair<double, pair<int, int>>, vector<pair<double, pair<int, int>>>, decltype(porownanie)> zbiorOtwarte(porownanie);

    gWynik[1 << start][start] = 0;
    fWynik[1 << start][start] = heurystyka(graf, start);
    zbiorOtwarte.push({fWynik[1 << start][start], {1 << start, start}});

    double najlepszyKoszt = numeric_limits<double>::infinity();
    vector<int> najlepszaSciezka;

    //auto czas_poczatkowy = chrono::high_resolution_clock::now();

    //auto czas_poczatkowy = chrono::high_resolution_clock::now();

    while (!zbiorOtwarte.empty()) {
//        auto obecny_czas = chrono::high_resolution_clock::now();
//        double miniony_czas = chrono::duration<double>(obecny_czas - czas_poczatkowy).count();


        auto [f_obecny, obecnyStan] = zbiorOtwarte.top();
        zbiorOtwarte.pop();
        int maska = obecnyStan.first;
        int wierzcholek = obecnyStan.second;

        // jesli wszystkie wierzchołki są odwiedzone, wróć do startu
        if (maska == (1 << n) - 1) {
            double obecnyKoszt = gWynik[maska][wierzcholek] + graf[wierzcholek][start];
            if (obecnyKoszt < najlepszyKoszt) {
                najlepszyKoszt = obecnyKoszt;
                najlepszaSciezka.clear();
                int obecny = wierzcholek;
                int obecnaMaska = maska;
                while (obecny != -1) {
                    najlepszaSciezka.push_back(obecny);
                    int poprzedni = poprzednik[obecnaMaska][obecny];
                    obecnaMaska &= ~(1 << obecny);
                    obecny = poprzedni;
                }
                reverse(najlepszaSciezka.begin(), najlepszaSciezka.end());
                najlepszaSciezka.push_back(start);

                // WARUNEK 30 MINUT

//                if (miniony_czas > 1800.0) {
//                    cout << "Przekroczono 30 minut" << endl;
//                    wierzcholki_sciezki_minimalnej = najlepszaSciezka;
//                    return gWynik[maska][wierzcholek] + graf[wierzcholek][start];
//                }

            }


            // Zrekonstruuj ścieżkę
            wierzcholki_sciezki_minimalnej.clear();
            int obecny = wierzcholek;
            int obecnaMaska = maska;
            while (obecny != -1) {
                wierzcholki_sciezki_minimalnej.push_back(obecny);
                int poprzedni = poprzednik[obecnaMaska][obecny];
                obecnaMaska &= ~(1 << obecny); // Usuwamy obecny wierzchołek z maski
                obecny = poprzedni;
            }
            reverse(wierzcholki_sciezki_minimalnej.begin(), wierzcholki_sciezki_minimalnej.end());
            wierzcholki_sciezki_minimalnej.push_back(start);
            return gWynik[maska][wierzcholek] + graf[wierzcholek][start];
        }

        // Przejdź przez wszystkich sąsiadów
        for (int sasiad = 0; sasiad < n; ++sasiad) {
            if (!(maska & (1 << sasiad))) { // jesli sąsiad nie jest odwiedzony
                int nowaMaska = maska | (1 << sasiad);
                double tymczasowy_gWynik = gWynik[maska][wierzcholek] + graf[wierzcholek][sasiad];

                if (tymczasowy_gWynik < gWynik[nowaMaska][sasiad]) {
                    gWynik[nowaMaska][sasiad] = tymczasowy_gWynik;
                    fWynik[nowaMaska][sasiad] = tymczasowy_gWynik + heurystyka(graf, sasiad);
                    poprzednik[nowaMaska][sasiad] = wierzcholek;
                    zbiorOtwarte.push({fWynik[nowaMaska][sasiad], {nowaMaska, sasiad}});
                }
            }
        }
    }

    return numeric_limits<double>::infinity(); // brak rozwiązania
}

/*
 double DFSBNB(std::vector<std::vector<double>> graf, int start, std::vector<int>& wierzcholki_sciezki_minimalnej, double wartoscOptymalna) {
    int N = graf.size();
    std::vector<int> obecna_sciezka; // obecna sciezka DFS
    std::vector<bool> odwiedzone(N, false); // odwiedzone wierzcholki
    double final_res = std::numeric_limits<double>::max(); // najlepszy koszt
    std::vector<int> najlepsza_sciezka; // sciezka o najmniejszym koszcie

    auto start_time = std::chrono::high_resolution_clock::now();

    // funkcja lambda DFS
    std::function<void(int, double)> DFS = [&](int wierzcholek, double obecny_koszt) {
        auto current_time = std::chrono::high_resolution_clock::now();
        double elapsed_time = std::chrono::duration<double>(current_time - start_time).count();
        if (elapsed_time > 1800.0) { // WARUNEK 30 MINUT
            return;
        }

        // dodajemy wierzcholek do obecnej sciezki
        obecna_sciezka.push_back(wierzcholek);
        odwiedzone[wierzcholek] = true;

        // jesli odwiedzilismy wszystkie wierzcholki i wracamy do poczatkowego
        if (obecna_sciezka.size() == N) {
            if (graf[wierzcholek][start] != 0) { // jesli istnieje krawedz powrotna
                double pelny_koszt = obecny_koszt + graf[wierzcholek][start];
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
        } else { // odwiedzamy sasiednie wierzcholki
            for (int i = 0; i < N; i++) {
                if (!odwiedzone[i] && graf[wierzcholek][i] != 0) {
                    DFS(i, obecny_koszt + graf[wierzcholek][i]);

                    // jesli znaleziono rozwiazanie optymalne przerywamy
                    if (final_res <= wartoscOptymalna) {
                        return;
                    }
                }
            }
        }

        // usuwamy wierzcholek z obecnej sciezki i cofamy sie
        odwiedzone[wierzcholek] = false;
        obecna_sciezka.pop_back();
    };

    // rozpoczynamy DFS od wierzcholka startowego
    DFS(start, 0);

    // przepisujemy najlepsza sciezke do wyniku, jezeli znaleziono jakies rozwiazanie
    if (!najlepsza_sciezka.empty()) {
        wierzcholki_sciezki_minimalnej = najlepsza_sciezka;
    }

    return final_res;
}
 */

