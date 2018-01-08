#include "lorPassword.h"

#include "mbedtls\pkcs5.h"
#include "lorString.h"
#include "lorTime.h"

enum
{
  DefaultAlgorithmIndex = 1,
  DefaultSaltLength = 16,
  DefaultPasswordHashLength = 32,
  DefaultComplexity = 5,

  PasswordIterationMultiply = 10000,
};

const char passwordAlphabet[] = "0123456789ABCDEF";

bool lorPassword_HashInternal(uint8_t algorithm, const char *pPassword, uint32_t passwordLength, const char *pSalt, uint32_t saltLength, int complexity, char *pHashBuffer, uint32_t hashBufferLength)
{
  if (algorithm != DefaultAlgorithmIndex)
    return false;

  //Set up the context
  mbedtls_md_context_t ctx;
  mbedtls_md_init(&ctx);

  int mdStatus = mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(MBEDTLS_MD_SHA256), 1);
  if (mdStatus != 0)
  {
    lorLog("md failed to init!");
    return false;
  }

  int status = mbedtls_pkcs5_pbkdf2_hmac(&ctx, (const unsigned char*)pPassword, passwordLength, (const unsigned char*)pSalt, saltLength, complexity * PasswordIterationMultiply, hashBufferLength, (unsigned char*)pHashBuffer);
  mbedtls_md_free(&ctx);

  if (status == 0)
    return true;

  return false;
}

const char* lorPassword_Hash(const char *pPassword)
{
  uint32_t passwordLength = (uint32_t)lorStrlen(pPassword);

  //Generate a salt
  uint64_t saltRNGSeed = lorTime_GetCurrentTimestamp();
  uint8_t salt[DefaultSaltLength];
  for (int i = 0; i < DefaultSaltLength; ++i)
  {
    salt[i] = (uint8_t)lorRandom(saltRNGSeed);
  }

  char hashedPassword[DefaultPasswordHashLength];
  memset(hashedPassword, 0, sizeof(hashedPassword));

  //+6 for 5x'$' & '\0', version & complexity. Salt and password get hex encoded so double in size
  char returnHash[DefaultSaltLength*2 + DefaultPasswordHashLength*2 + 6 + (DefaultAlgorithmIndex / 16 + 1) + (DefaultComplexity / 16 + 1)];
  memset(returnHash, 0, sizeof(returnHash));

  if (lorPassword_HashInternal(DefaultAlgorithmIndex, pPassword, passwordLength, (char*)salt, DefaultSaltLength, DefaultComplexity, hashedPassword, DefaultPasswordHashLength))
  {
    int carat = lorSprintf(returnHash, sizeof(returnHash), "$%X$%X$", DefaultAlgorithmIndex, DefaultComplexity);

    for (int i = 0; i < DefaultSaltLength; ++i)
    {
      char nextChar = salt[i];
      returnHash[carat++] = passwordAlphabet[((nextChar >> 4) & 0xF)];
      returnHash[carat++] = passwordAlphabet[(nextChar & 0xF)];
    }

    returnHash[carat++] = '$';

    for (int i = 0; i < DefaultPasswordHashLength; ++i)
    {
      char nextChar = hashedPassword[i];
      returnHash[carat++] = passwordAlphabet[((nextChar >> 4) & 0xF)];
      returnHash[carat++] = passwordAlphabet[(nextChar & 0xF)];
    }

    returnHash[carat++] = '$';
    returnHash[carat] = '\0';
  }
  else
  {
    return nullptr;
  }

  return lorStrdup(returnHash);
}

bool lorPassword_Verify(const char *pPassword, const char *pHash, bool *pRequiresRehash /*= nullptr*/)
{
  if (pRequiresRehash != nullptr)
    *pRequiresRehash = false; //Does not require a rehash unless the password is correct AND the method is out of date

  if (pPassword == nullptr || pHash == nullptr)
    return false;

  uint32_t passwordLength = (uint32_t)lorStrlen(pPassword);

  char *pHashDupPtr = lorStrdup(pHash);
  char *pHashDup = pHashDupPtr;

  bool retVal = false;
  uint8_t dollarCount = 0;

  char *pAlgo = nullptr;
  char *pComplexity = nullptr;
  char *pSalt = nullptr;
  char *pPwdHash = nullptr;

  //lets verify this is a valid hash & split out the strings
  while(*pHashDup != '\0')
  {
    if (*pHashDup == '$')
    {
      ++dollarCount;
      *pHashDup = '\0';

      switch (dollarCount)
      {
      case 1:
        pAlgo = pHashDup + 1;
        break;
      case 2:
        pComplexity = pHashDup + 1;
        break;
      case 3:
        pSalt = pHashDup + 1;
        break;
      case 4:
        pPwdHash = pHashDup + 1;
        break;
      case 5:
        if (*(pHashDup + 1) == '\0')
          break;
        else
          goto epilogue; //Badly formed hash string
      default:
        goto epilogue; //Shouldn't be possible to get here but in case
      }
    }
    else if (((*pHashDup) < '0' || (*pHashDup) > '9') && ((*pHashDup) < 'A' || (*pHashDup > 'F')))
    {
      goto epilogue; //Invalid character
    }

    ++pHashDup;
  }

  if (pAlgo == nullptr || pAlgo[0] == '\0' || pComplexity == nullptr || pComplexity[0] == '\0' || pSalt == nullptr || pSalt[0] == '\0' || pPwdHash == nullptr || pPwdHash[0] == '\0')
    goto epilogue;

  uint8_t algorithmVersion = (uint8_t)lorStrAtoI(pAlgo, 16);
  int complexity = lorStrAtoI(pComplexity, 16);

  if (algorithmVersion > 0 && complexity > 0)
  {
    //These get decoded to half the size
    uint32_t saltLength = (uint32_t)lorStrlen(pSalt) / 2;
    uint32_t pwdHashLength = (uint32_t)lorStrlen(pPwdHash) / 2;

    char *pSaltBinary = lorAllocType(char, saltLength);
    char *pPwdHashBinaryOld = lorAllocType(char, pwdHashLength);
    char *pPwdHashBinaryNew = lorAllocType(char, pwdHashLength);

    for (uint32_t i = 0; i < saltLength; ++i)
    {
      char *pCS = &pSalt[i * 2];

      int8_t bits0 = (pCS[0] >= '0' && pCS[0] <= '9') ? (pCS[0] - '0') : 10 + (pCS[0] - 'A');
      int8_t bits1 = (pCS[1] >= '0' && pCS[1] <= '9') ? (pCS[1] - '0') : 10 + (pCS[1] - 'A');

      pSaltBinary[i] = (bits0 << 4) | bits1;
    }

    for (uint32_t i = 0; i < pwdHashLength; ++i)
    {
      char *pCS = &pPwdHash[i * 2];

      int8_t bits0 = (pCS[0] >= '0' && pCS[0] <= '9') ? (pCS[0] - '0') : 10 + (pCS[0] - 'A');
      int8_t bits1 = (pCS[1] >= '0' && pCS[1] <= '9') ? (pCS[1] - '0') : 10 + (pCS[1] - 'A');

      pPwdHashBinaryOld[i] = (bits0 << 4) | bits1;
    }

    //Decode salt and encode pPassword
    if (lorPassword_HashInternal(algorithmVersion, pPassword, passwordLength, pSaltBinary, saltLength, complexity, pPwdHashBinaryNew, pwdHashLength))
    {
      if (memcmp(pPwdHashBinaryNew, pPwdHashBinaryOld, pwdHashLength) == 0)
      {
        retVal = true;
        if (pRequiresRehash != nullptr && (algorithmVersion != DefaultAlgorithmIndex || complexity != DefaultComplexity))
          *pRequiresRehash = true; //Does not need a rehash
      }
    }

    lorFree(pSaltBinary);
    lorFree(pPwdHashBinaryOld);
    lorFree(pPwdHashBinaryNew);
  }

epilogue:
  lorFree(pHashDupPtr);

  return retVal;
}
