

def can_build(env, platform):
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "TerraWorldArea",

        "TerraLight",
        "TerramanLight",

        "TerramanLevelGenerator",
        "TerramanLevelGeneratorFlat",

        "TerraSurfaceMerger",
        "TerraSurfaceSimple",
        "TerraSurface",
        "TerramanLibraryMerger",
        "TerramanLibrarySimple",
        "TerramanLibrary",
        "TerramanLibraryMergerPCM",

        "TerraMaterialCache",

        "TerraCubePoints",
        "TerraMesherCubic",
        "TerraMeshData",

        "MarchingCubesCellData",
        "TerraMesherMarchingCubes",

        "TerraMesher",

        "EnvironmentData",
        "TerraChunk",
        "TerraChunkDefault",
        "TerraStructure",
        "BlockTerraStructure",
        "TerraWorld",

        "TerraMesherBlocky",
        "TerraWorldBlocky",
        "TerraChunkBlocky",
        "TerraMesherLiquidBlocky",

        "TerraWorldMarchingCubes",
        "TerraChunkMarchingCubes",
    
        "TerraMesherCubic",
        "TerraWorldCubic",
        "TerraChunkCubic",

        "TerraMesherDefault",
        "TerraWorldDefault",

        "TerraJob",
        "TerraTerrarinJob",
        "TerraLightJob",
        "TerraPropJob",
    ]


def get_doc_path():
    return "doc_classes"
