#ifndef lorPassword_h__
#define lorPassword_h__

//lorPassword hashes have the form: $<algorithm>$<complexity>$<salt>$<hash>$
//  there will always be ONLY 5 '$' symbols with all four variables being base 16 encoded
//  Note that algorithm does not follow the numbering convention other systems use and complexity is algorithm specific

const char* lorPassword_Hash(const char *pPassword); // Creates a password hash with the current 'best' options, the return will need to be freed
bool lorPassword_Verify(const char *pPassword, const char *pHash, bool *pRequiresRehash = nullptr); //Verifies a raw password against a hash

#endif
