from PIL import Image, ImageOps

filenames = ["palmeiras.png", "flarj.png", "gremio.png", "santos.png", 
            "riverplate_arg.png", "bocajuniors_arg.png", "nacional_uru.png", 
            "penarol_uru.png", "nacional_col.png", "bayern_ale.png", 
            "realmadrid_esp.png", "machestercity_ing.png", "barcelona_esp.png", 
            "juventus_it.png", "atleticomadrid_esp.png", "machester_ing.png", 
            "parissaintgermain_fr.png", "liverpool_ing.png", "chelsea.png", 
            "inter_it.png", "borussia_ale.png", "melbournecityfc_aus.png", 
            "gambaosaka_jap.png", "atleticomg_bra.png", "guangzhou_chn.png", 
            "america_mex.png", "tigres_mex.png", "atlantaunited_eua.png", 
            "corinthians_bra.png", "alahly_egi.png", "rajacasablanca_mar.png", 
            "internacional_bra.png"]
for team in filenames:
    with Image.open(f"lugar\\{team}") as im:
        w, h = im.size
        print(w, h)
        img = ImageOps.expand(im, border=(10, 0, 10, 0), fill=(0, 0, 0, 0))
        img.save(f"lugar\\new\\{team}")
