TOP=$(abspath ../..)
include $(TOP)/defs.mak

ifdef STRACE
OPTS = --strace
endif

APPDIR=$(CURDIR)/appdir
APPBUILDER=$(TOP)/scripts/appbuilder

ROOTFS=$(CURDIR)/rootfs

export TIMEOUT=10000

all: myst $(ROOTFS)

$(APPDIR):
	$(APPBUILDER) Dockerfile

$(ROOTFS): $(APPDIR)
	$(MYST) mkext2 $(APPDIR) $(ROOTFS)
	truncate --size=-4096 $(ROOTFS)

OPTS += --app-config-path config.json

tests: all
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/template/azure-template/test/azure-template-test
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/core/azure-core/test/ut/azure-core-global-context-test
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/keyvault/azure-security-keyvault-keys/test/ut/azure-security-keyvault-keys-test
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/identity/azure-identity/test/live/azure-identity-livetest
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/keyvault/azure-security-keyvault-secrets/test/ut/azure-security-keyvault-secrets-test
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/core/azure-core/test/fault-injector/azure-core-test-fault-injector

	# Run partial test; the ones in the filter are disabled
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/core/azure-core/test/ut/azure-core-test --gtest_filter=-BearerTokenAuthenticationPolicy.RefreshNearExpiry:*BodyStream.Rewind:*BodyStream.BadInput:*Pre*ondition*:CurlConnectionPool.*onnectionClose*:*ExpectThrow*:Context.IsCancelled
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/identity/azure-identity/test/ut/azure-identity-test --gtest_filter=-*ClientSecret*:ManagedIdentityCredential* # timing problem
	$(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/storage/azure-storage-test --gtest_filter=-*ExpectThrow*:*Blob*:*ConcurrentUploadDownload:*DataLakeSasTest:*FileShare*:StoragetimeoutTest*:DataLake*ClientTest*:*ClientSecretCredentialWorks # needs fork & STANDARD_STORAGE_CONNECTION_STRING

	# FAIL OUTSIDE MYSTIKOS currently
	# $(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/core/azure-core/test/ut/azure-core-libcurl-test
	# $(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/keyvault/azure-security-keyvault-keys/test/ut/azure-security-keyvault-keys-test-live
	# $(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk/keyvault/azure-security-keyvault-secrets/test/sample/keyvault-secrets-test-app

myst:
	$(MAKE) -C $(TOP)/tools/myst

clean:
	rm -rf $(APPDIR) $(ROOTFS)

get-executables:
	find $(RUNTEST) $(MYST_EXEC) $(OPTS) $(ROOTFS) /azure-sdk-for-cpp/build/sdk -type f -executable -print | grep test
