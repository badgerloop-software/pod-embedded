import util

# Generates the data.h file
def generateDataHeader(data):
    out = ""
    # Generate initializers
    for field in data.iter("struct"):
        if "init" not in field.attrib:
            continue
        out += "int " + field.attrib["init"] + "(void);\n"  

    out += "\n"
    # Generate data structures
    for struct in data.iter("struct"):
        if "comment" in struct.attrib:
            out += util.generateComment(struct.attrib["comment"], 100)
        out += "typedef struct " + struct.attrib["id"] + "_t {\n"
        for field in struct:
            if "comment" in field.attrib:
                out += util.generateComment(field.attrib["comment"], 100, "\t")
            fieldId = ("*" if ("pointer" in field.attrib and field.attrib["pointer"] == "true") else "") + field.attrib["id"]
            if field.tag == "struct":
                out += "\tstruct " + field.attrib["id"] + "_t " + fieldId + ";\n"
            elif field.tag == "field":
                fieldType = field.attrib["type"]
                # Deal with arrays
                if field.attrib["type"].endswith("]"):
                    fieldId += "[" + field.attrib["type"].split("[")[1]
                    fieldType = field.attrib["type"].split("[")[0]
                out += "\t" + fieldType + " " + fieldId + ";\n"
        out += "} " + struct.attrib["id"] + "_t;\n\n"
    return out

def generateBufferContents(data):
    out = ""
    for field in data.iter("field"):
        out += "addToBuffer(&buffer, &" + util.getDataReference(field, data) + ");\n"
    return out