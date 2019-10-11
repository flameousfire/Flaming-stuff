import random
import math
import time
import datetime
import haravasto
import ikkunasto as ik

palikat = {}

parametrit = {
    "korkeus": 10,
    "leveys": 10,
    "miinalkm": 10
}

tila = {
    "kentta_nakymaton": None,
    "kentta": None,
    "jaljella": None,
    "avaamattomat": parametrit["korkeus"] * parametrit["leveys"],
    "klikkaukset": None,
    "aika": None
}

#Peliin liittyvät funktiot

def luo_kentta(leveys, korkeus):
    kentta = []
    for rivi in range(korkeus):
        kentta.append([])
        for sarake in range(leveys):
            kentta[-1].append(" ")

    tila["kentta"] = kentta
    
    kentta2 = []
    for rivi in range(korkeus):
        kentta2.append([])
        for sarake in range(leveys):
            kentta2[-1].append(" ")
    tila["kentta_nakymaton"] = kentta2

    jaljella = []
    for x in range(parametrit["leveys"]):
        for y in range(parametrit["korkeus"]):
            jaljella.append((x, y))
    tila["jaljella"] = jaljella
    
def miinoita(koordinaatisto, vapaat, n):
    for i in random.sample(vapaat, n):
        x, y = i
        koordinaatisto[y][x] = "x"

def tutki_kentta(ruudukko):
    for y, rivi in enumerate(ruudukko):
        for x, alkio in enumerate(rivi):
            if ruudukko[y][x] == "x":
                pass
            else:
                ruudukko[y][x] = laske_miinat(x, y, ruudukko)
                if ruudukko[y][x] == 0:
                    ruudukko[y][x] = str(0)
    
def laske_miinat(x, y, ruudukko):
    n = 0
    for a in range(max(0, y - 1), min(len(ruudukko), y + 2)):
        for b in range(max(0, x - 1), min(x + 2, len(ruudukko[0]))):
            c = ruudukko[a][b]
            if c == "x":
                n = n + 1
    if ruudukko[y][x] == "x":
        n = n - 1
    return n
    
def tulvataytto(kentta, ax, ay):
    if kentta[ay][ax] == "x":
        pass
    else:
        tulvalista = []
        tulvalista.append([ax, ay])
        while tulvalista != []:
            i, j = tulvalista.pop()
            for a in range(max(0, j - 1), min(len(kentta), j + 2)):
                for b in range(max(0, i - 1), min(i + 2, len(kentta[0]))):
                    if tila["kentta"][a][b] == " " and tila["kentta_nakymaton"][a][b] == "0":
                        tulvalista.append([b, a])
                    kentta[a][b] = tila["kentta_nakymaton"][a][b]    
        n = 0
        for rivi in kentta:
            n += rivi.count(" ")
            n += rivi.count("f")
        tila["avaamattomat"] = n

"""Pelin toiminnan kannalta keskeisin funktio. "Kääntää" ruudun. Miinaan osuessa tappio, "0" ruudun tapauksessa tutkitaan viereiset (jne.) ja jos on avattu kaikki paitsi miinalliset ruudut, voitto."""      
def tutki_ruutu(x, y):
    tila["avaamattomat"] -= 1
    if tila["kentta_nakymaton"][y][x] == "x":
        havio()
    else:
        tila["kentta"][y][x] = tila["kentta_nakymaton"][y][x]
        if tila["kentta_nakymaton"][y][x] == "0":
            tulvataytto(tila["kentta"], x, y)
    if tila["avaamattomat"] - parametrit["miinalkm"] == 0:
        voitto()
        
def havio():
    tila["aika"] = round(time.time(), 2) - tila["aika"]
    tila["kentta"] = tila["kentta_nakymaton"]
    piirra_kentta()
    min = int(tila["aika"] // 60)
    sek = tila["aika"] % 60
    tappioviesti = "Hävisit pelin :( \nAikaa kului {} minuuttia ja {:.2f} sekuntia. \nKlikkasit {} kertaa.".format(min, sek, tila["klikkaukset"])
    ik.avaa_viesti_ikkuna("Tappio", tappioviesti)
    tilastoi_tappio()
    
def voitto():
    tila["aika"] = round(time.time(), 2) - tila["aika"]
    min = int(tila["aika"] // 60)
    sek = tila["aika"] % 60
    voittoviesti = "Voitit pelin :) \nAikaa kului {} minuuttia ja {:.2f} sekuntia. \nKlikkasit {} kertaa.".format(min, sek, tila["klikkaukset"])
    ik.avaa_viesti_ikkuna("Voitto", voittoviesti)
    tilastoi_voitto()

"""Funktio joka aloittaa pelin. Ensin se luo kaksi kenttää, joista toinen on näkyvä ja toinen piilotettu. Piilotettu kenttä miinoitetaan ja tutkitaan, eli lasketaan ruutujen viereiset miinat. 
Sitten kutsutaan haravaston tarpeelliset grafiikkafunktiot ja käynnistetään peli. (Ja "kello") """
def main():
    luo_kentta(parametrit["leveys"], parametrit["korkeus"])
    miinoita(tila["kentta_nakymaton"], tila["jaljella"], parametrit["miinalkm"])
    tutki_kentta(tila["kentta_nakymaton"])
    haravasto.lataa_kuvat("spritet")
    haravasto.luo_ikkuna(parametrit["leveys"] * 40, parametrit["korkeus"] * 40)
    haravasto.aseta_piirto_kasittelija(piirra_kentta)
    haravasto.aseta_hiiri_kasittelija(kasittele_hiiri)
    tila["aika"] = round(time.time(), 2)
    tila["klikkaukset"] = 0
    haravasto.aloita()

#Grafiikkaan liittyvät funktiot

def piirra_kentta():
    haravasto.tyhjaa_ikkuna()
    haravasto.piirra_tausta()
    haravasto.aloita_ruutujen_piirto()
    for rivi, a in enumerate(tila["kentta"]):
        for ruutu, b in enumerate(a):
            haravasto.lisaa_piirrettava_ruutu(tila["kentta"][rivi][ruutu], ruutu * 40, rivi * 40)
    haravasto.piirra_ruudut()

def kasittele_hiiri(xi, yi, painike, muokkaus):
    x = math.floor(xi / 40)
    y = math.floor((yi) / 40)
    if painike == haravasto.HIIRI_OIKEA:
        if tila["kentta"][y][x] == " ":
            tila["kentta"][y][x] = "f"
        elif tila["kentta"][y][x] == "f":  
            tila["kentta"][y][x] = " "
        else:
            pass
    elif painike == haravasto.HIIRI_VASEN:
        if tila["kentta"][y][x] == " ":
            tila["klikkaukset"] += 1
            tutki_ruutu(x, y)
    elif painike == haravasto.HIIRI_KESKI:
        tila["klikkaukset"] += 1
        for a in range(max(0, y - 1), min(len(tila["kentta"]), y + 2)):
                for b in range(max(0, x - 1), min(x + 2, len(tila["kentta"][0]))):
                    if tila["kentta"][a][b] == " ":
                        tutki_ruutu(b, a)
    else:                
        pass
    piirra_kentta()

#Valikkoon liittyvät funktiot

def tilasto():
    try:
        with open("tilasto.txt") as luku:
            sisalto = luku.readlines()
            pelit, voitot, tappiot = sisalto[0].split(",")
            voitot = int(voitot)
            tappiot = int(tappiot.strip(" "))
            pelit = int(pelit)
            if pelit == 0:
                voittoprosentti = 0
            else:
                voittoprosentti = voitot / pelit * 100
            aika1, pvm1 = sisalto[1].split("p")
            aika2, pvm2 = sisalto[2].split("p")
            aika3, pvm3 = sisalto[3].split("p")
            pvm1 = pvm1.strip(" ")
            pvm2 = pvm2.strip(" ")
        return "Pelatut pelit: {} \nVoitot: {} \nTappiot: {} \nVoittoprosentti: {:.2f} \n\nParhaat ajat: \n\n\
10 x 10 ruudukko, 10 miinaa: \nNopein aika {}s pelattu {} \n20 x 20 ruudukko, 50 miinaa: \
    \nNopein aika  {}s pelattu {} \n25 x 25 ruudukko, 100 miinaa: \nNopein aika  {}s pelattu {}\
    ".format(pelit, voitot, tappiot, voittoprosentti, aika1, pvm1, aika2, pvm2, aika3, pvm3)
    except IOError:
        ik.avaa_viesti_ikkuna("Virhe", "Ole hyvä ja nollaa tilastot", virhe=True)
        return "Ei tilastoja."

def tilastoi_voitto():
    try:
        with open("tilasto.txt") as luku:
            sisalto = luku.readlines()
            pelit, voitot, tappiot = sisalto[0].split(",")
            voitot = int(voitot)
            voitot += 1
            pelit = int(pelit)
            pelit += 1
            tappiot = tappiot.strip()
            aika1, pvm1 = sisalto[1].split("p")
            aika2, pvm2 = sisalto[2].split("p")
            aika3, pvm3 = sisalto[3].split("p")
            aika1 = float(aika1)
            aika2 = float(aika2)
            aika3 = float(aika3)
            pvm1 = pvm1.strip()
            pvm2 = pvm2.strip()
            if parametrit["korkeus"] == 10 and parametrit["leveys"] == 10 and parametrit["miinalkm"] == 10:
                if tila["aika"] < aika1:
                    aika1 = tila["aika"]
                    pvm1 = datetime.datetime.today()
            elif parametrit["korkeus"] == 20 and parametrit["leveys"] == 20 and parametrit["miinalkm"] == 50:
                if tila["aika"] < aika2:
                    aika2 = tila["aika"]
                    pvm2 = datetime.datetime.today()
            elif parametrit["korkeus"] == 25 and parametrit["leveys"] == 25 and parametrit["miinalkm"] == 100:
                if tila["aika"] < aika3:
                    aika3 = tila["aika"]
                    pvm3 = datetime.datetime.today()
        with open("tilasto.txt", "w") as kohde:
            kohde.write("{},{},{}\n".format(pelit, voitot, tappiot))
            kohde.write("{:.2f}p{}\n".format(aika1, pvm1))
            kohde.write("{:.2f}p{}\n".format(aika2, pvm2))
            kohde.write("{:.2f}p{}".format(aika3, pvm3))
    except IOError:
        ik.avaa_viesti_ikkuna("Virhe", "Ole hyvä ja nollaa tilastot", virhe=True)
            
def tilastoi_tappio():
    try:
        with open("tilasto.txt") as tilasto:
            sisalto = tilasto.readlines()
            pelit, voitot, tappiot = sisalto[0].split(",")
            tappiot = tappiot.strip()
            tappiot = int(tappiot)
            tappiot += 1
            pelit = int(pelit)
            pelit += 1
            aika1, pvm1 = sisalto[1].split("p")
            aika2, pvm2 = sisalto[2].split("p")
            aika3, pvm3 = sisalto[3].split("p")
            aika1 = float(aika1)
            aika2 = float(aika2)
            aika3 = float(aika3)
            pvm1 = pvm1.strip()
            pvm2 = pvm2.strip()
 
        with open("tilasto.txt", "w") as tilasto:
            tilasto.write("{},{},{}\n".format(pelit, voitot, tappiot))
            tilasto.write("{:.2f}p{}\n".format(aika1, pvm1))
            tilasto.write("{:.2f}p{}\n".format(aika2, pvm2))
            tilasto.write("{:.2f}p{}".format(aika3, pvm3))
    except IOError:
        ik.avaa_viesti_ikkuna("Virhe", "Ole hyvä ja nollaa tilastot", virhe=True)

def nollaus():
    pelit = 0
    voitot = 0
    tappiot = 0
    aika = 999999
    pvm = 0
    with open("tilasto.txt", "w") as kohde:
        kohde.write("{},{},{}\n".format(pelit, voitot, tappiot))
        kohde.write("{:.2f}p{}\n".format(aika, pvm))
        kohde.write("{:.2f}p{}\n".format(aika, pvm))
        kohde.write("{:.2f}p{}".format(aika, pvm))
    
    ik.kirjoita_tekstilaatikkoon(palikat["tilastolaatikko"], tilasto(), True)
    
def tilastot():
    palikat["tilasto"] = ik.luo_ali_ikkuna("Tilastot")
    ylakehys = ik.luo_kehys(palikat["tilasto"], ik.YLA)
    alakehys = ik.luo_kehys(palikat["tilasto"], ik.ALA)
    nollaa = ik.luo_nappi(alakehys, "Nollaa tilastot", nollaus)
    palikat["tilastolaatikko"] = ik.luo_tekstilaatikko(ylakehys, 55, 25)
    ik.kirjoita_tekstilaatikkoon(palikat["tilastolaatikko"], tilasto())
    
def valinnat():
    palikat["valinnat"] = ik.luo_ali_ikkuna("Valinnat")
    ylakehys = ik.luo_kehys(palikat["valinnat"], ik.YLA)
    syotekehys = ik.luo_kehys(ylakehys, ik.VASEN)
    nappikehys = ik.luo_kehys(ylakehys, ik.VASEN)
    korkeusohje = ik.luo_tekstirivi(syotekehys, "Korkeus:")
    palikat["korkeuskentta"] = ik.luo_tekstikentta(syotekehys)
    leveysohje = ik.luo_tekstirivi(syotekehys, "Leveys:")
    palikat["leveyskentta"] = ik.luo_tekstikentta(syotekehys)
    miinatohje = ik.luo_tekstirivi(syotekehys, "Miinojen lukumäärä:")
    palikat["miinatkentta"] = ik.luo_tekstikentta(syotekehys)
    oknappi = ik.luo_nappi(nappikehys, "Ok", ok)
    
    
def ok():    
    try:
        parametrit["korkeus"] = int(ik.lue_kentan_sisalto(palikat["korkeuskentta"]))
        parametrit["leveys"] = int(ik.lue_kentan_sisalto(palikat["leveyskentta"]))
        parametrit["miinalkm"] = int(ik.lue_kentan_sisalto(palikat["miinatkentta"]))
        ik.piilota_ali_ikkuna(palikat["valinnat"])
    except ValueError:
        ik.avaa_viesti_ikkuna("Virhe", "Syötä kolme kokonaislukua", virhe=True)


#Pääohjelma

valikko_ikkuna = ik.luo_ikkuna("Miinaharava")
ylakehys = ik.luo_kehys(valikko_ikkuna, ik.YLA)
alakehys = ik.luo_kehys(valikko_ikkuna, ik.YLA)
nappikehys = ik.luo_kehys(ylakehys, ik.VASEN)
pelinappi = ik.luo_nappi(nappikehys, "Aloita uusi peli", main)
valinnatnappi = ik.luo_nappi(nappikehys, "Valinnat", valinnat)
tilastotnappi = ik.luo_nappi(nappikehys, "Tilastot", tilastot)
lopetusnappi = ik.luo_nappi(nappikehys, "Lopeta", ik.lopeta)
ik.kaynnista()


