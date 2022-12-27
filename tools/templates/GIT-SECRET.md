<!--
Maintainer:   jeffskinnerbox@yahoo.com / www.jeffskinnerbox.me
Version:      0.9.5
-->


<div align="center">
<img src="http://www.foxbyrd.com/wp-content/uploads/2018/02/file-4.jpg" title="These materials require additional work and are not ready for general use." align="center">
</div>


-----


# Git-Secret
The `ntp-clock` needs WiFi credentials to operate,
so how do I store them out on GitHub without exposing my home WiFi to Internet riffraff?
The tool [`git-secret`][01] (GitHub repository [here][02]) helps solve this problem.
`git-secret` is a shell utility to store your private data inside a `git` repository.
It encrypts using `gpg` and the public keys of all the users that you trust.
So everyone of them can decrypt these files using only their personal secret key.
There are no passwords that change.
When someone leaves your circle of trust, just delete their public key,
re-encrypt the files, and they won’t be able to decrypt secrets anymore.

# Install
There are multiple method for installing `git-secret` (see [here][03]),
but I choose to load the source locally on my computer.

```bash
# git-secret relies on two dependencies, git and gpg, so install them first
sudo apt-get install gnupg git

# install git-secret source
cd ~/src
git clone https://github.com/sobolevn/git-secret.git git-secret
cd git-secret
make build
PREFIX="/home/jeff" make install
```

Before you start using `git-secret`,
make sure you have created a gpg RSA key-pair (i.e. public and secret key)
assoicated with the email address your using with your `git` repository.
So lets list the keys I have on my ring:

```bash
# list the keys in your public key ring
$ gpg --list-keys
/home/jeff/.gnupg/pubring.gpg
-----------------------------
pub   rsa2048 2013-07-20 [SC] [expires: 2020-07-18]
      01E2EFB6F7CE17A5806A69154B0A1A75F2CA7A17
uid           [ unknown] Stuart D. Gathman (RPM signing key) <stuart@gathman.org>
sub   rsa2048 2013-07-20 [E] [expires: 2020-07-18]

pub   rsa4096 2016-04-10 [SC] [expired: 2018-04-10]
      9B157FB3053AE25AFEA3F1679284614CD7801CE9
uid           [ expired] Jeff Irland (cjdns experimenting with Stuart Gathman) <jeff.irland@verizon.net>

pub   rsa4096 2012-03-05 [SC] [expires: 2021-02-03]
      44C6513A8E4FB3D30875F758ED444FF07D8D0BF6
uid           [ unknown] Kali Linux Repository <devel@kali.org>
sub   rsa4096 2012-03-05 [E] [expires: 2021-02-03]

# list the keys in your secret key ring
$ gpg --list-secret-keys
/home/jeff/.gnupg/pubring.gpg
-----------------------------
sec   rsa4096 2016-04-10 [SC] [expired: 2018-04-10]
      9B157FB3053AE25AFEA3F1679284614CD7801CE9
uid           [ expired] Jeff Irland (cjdns experimenting with Stuart Gathman) <jeff.irland@verizon.net>

```

My GitHub email is `jeffskinnerbox@yahoo.com`.
There is no key for this email, so let create one.

```bash
# generate a gpg key pair
$ gpg --full-generate-key
gpg (GnuPG) 2.2.12; Copyright (C) 2018 Free Software Foundation, Inc.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

Please select what kind of key you want:
   (1) RSA and RSA (default)
   (2) DSA and Elgamal
   (3) DSA (sign only)
   (4) RSA (sign only)
Your selection? 1
RSA keys may be between 1024 and 4096 bits long.
What keysize do you want? (3072) 4096
Requested keysize is 4096 bits
Please specify how long the key should be valid.
         0 = key does not expire
      <n>  = key expires in n days
      <n>w = key expires in n weeks
      <n>m = key expires in n months
      <n>y = key expires in n years
Key is valid for? (0) 0
Key does not expire at all
Is this correct? (y/N) y

GnuPG needs to construct a user ID to identify your key.

Real name: git-secret
Email address: jeffskinnerbox@yahoo.com
Comment: used to store credentials and other secrets on GitHub
You selected this USER-ID:
    "git-secret (used to store credentials and other secrets on GitHub) <jeffskinnerbox@yahoo.com>"

Change (N)ame, (C)omment, (E)mail or (O)kay/(Q)uit? O
We need to generate a lot of random bytes. It is a good idea to perform
some other action (type on the keyboard, move the mouse, utilize the
disks) during the prime generation; this gives the random number
generator a better chance to gain enough entropy.
We need to generate a lot of random bytes. It is a good idea to perform
some other action (type on the keyboard, move the mouse, utilize the
disks) during the prime generation; this gives the random number
generator a better chance to gain enough entropy.
gpg: key ED724D7E03FEBFFD marked as ultimately trusted
gpg: directory '/home/jeff/.gnupg/openpgp-revocs.d' created
gpg: revocation certificate stored as '/home/jeff/.gnupg/openpgp-revocs.d/0F7F17BF34CFE7F6147CA6F4ED724D7E03FEBFFD.rev'
public and secret key created and signed.

pub   rsa4096 2019-11-17 [SC]
      0F7F17BF34CFE7F6147CA6F4ED724D7E03FEBFFD
uid                      git-secret (used to store credentials and other secrets on GitHub) <jeffskinnerbox@yahoo.com>
sub   rsa4096 2019-11-17 [E]
```

Now lets check the key ring for updates:

```bash
# list the keys in your public key ring
$ gpg --list-keys
gpg: checking the trustdb
gpg: marginals needed: 3  completes needed: 1  trust model: pgp
gpg: depth: 0  valid:   2  signed:   0  trust: 0-, 0q, 0n, 0m, 0f, 2u
/home/jeff/.gnupg/pubring.gpg
-----------------------------
pub   rsa2048 2013-07-20 [SC] [expires: 2020-07-18]
      01E2EFB6F7CE17A5806A69154B0A1A75F2CA7A17
uid           [ unknown] Stuart D. Gathman (RPM signing key) <stuart@gathman.org>
sub   rsa2048 2013-07-20 [E] [expires: 2020-07-18]

pub   rsa4096 2016-04-10 [SC] [expired: 2018-04-10]
      9B157FB3053AE25AFEA3F1679284614CD7801CE9
uid           [ expired] Jeff Irland (cjdns experimenting with Stuart Gathman) <jeff.irland@verizon.net>

pub   rsa4096 2012-03-05 [SC] [expires: 2021-02-03]
      44C6513A8E4FB3D30875F758ED444FF07D8D0BF6
uid           [ unknown] Kali Linux Repository <devel@kali.org>
sub   rsa4096 2012-03-05 [E] [expires: 2021-02-03]

pub   rsa4096 2019-11-17 [SC]
      0F7F17BF34CFE7F6147CA6F4ED724D7E03FEBFFD
uid           [ultimate] git-secret (used to store credentials and other secrets on GitHub) <jeffskinnerbox@yahoo.com>
sub   rsa4096 2019-11-17 [E]

# list the keys in your secret key ring
$ gpg --list-secret-keys
/home/jeff/.gnupg/pubring.gpg
-----------------------------
sec   rsa4096 2016-04-10 [SC] [expired: 2018-04-10]
      9B157FB3053AE25AFEA3F1679284614CD7801CE9
uid           [ expired] Jeff Irland (cjdns experimenting with Stuart Gathman) <jeff.irland@verizon.net>

sec   rsa4096 2019-11-17 [SC]
      0F7F17BF34CFE7F6147CA6F4ED724D7E03FEBFFD
uid           [ultimate] git-secret (used to store credentials and other secrets on GitHub) <jeffskinnerbox@yahoo.com>
ssb   rsa4096 2019-11-17 [E]
```

With this, we are ready-to-go for using `git-secret`.

# git-secret Usage
Before using, make sure the install steps above were completed.
Make sure you have created gpg RSA key-pair: public and secret key identified by your email address.

## Step 1: Create the git-secret Repository
Initialize the `git-secret` repository by running `git secret init` command in your target project directory.
The `.gitsecret/` folder will be created.

>**NOTE:** All the contents of the `.gitsecret/` folder will need to be checked into your git repository,
>with the **exception** of the `random_seed` file.
>In other words, for the directory `.gitsecret`,
>only the random_seed file should be mentioned in your `.gitignore` file.
>`git-secret` will do all this automatically.

```bash
# go to your target directory
cd ~/src/ntp-clock

# create the git-secret's directory
git secret init

# check if .gitignore has been updated
$ tail -n 2 .gitignore
.gitsecret/keys/random_seed
!*.secret
```

## Step 2: Add Users
Add the first user (yourself) to the `git-secret` repo keyring
by running `git secret tell your@gpg.email`:

```bash
# add yourself to the git-secret keyring
$ git secret tell jeffskinnerbox@yahoo.com

gpg: keybox '/home/jeff/src/ntp-clock/.gitsecret/keys/pubring.kbx' created
gpg: /home/jeff/src/ntp-clock/.gitsecret/keys/trustdb.gpg: trustdb created
git-secret: done. jeffskinnerbox@yahoo.com added as user(s) who know the secret.
```

## Step 3: Add Encrypted Files
Now add files you wish to encrypt inside the `git-secret` repository.
It can be done by running `git secret add <filenames...>` command.
`git-secret` automatically doesn't allow you to add the secret file to the repository.

After you add all the files you wish to encrypt,
run `git secret hide` to encrypt all files.
The data will be encrypted with the public-keys described by
the `git secret tell your@gpg.email` command.

```bash
# encrypt file via git-secret
$ git secret add credentials.json
git-secret: 1 item(s) added.

# check if you can add it to the repository
$ git add credentials.json
The following paths are ignored by one of your .gitignore files:
credentials.json
Use -f if you really want to add them.

# doing an `git add -all` produces no message
git add --all

# now encrypt the hiden files
$ git secret hide
git-secret: done. 1 of 1 files are hidden.
```

After using `git secret hide` to encrypt your data,
it is safe to commit your changes.

>**NOTE:** It is recommended to add `git secret hide` command to your pre-commit hook,
>so you won’t miss any changes.

## Step 4: Revealing the Secret Later
You can decrypt files with the `git secret reveal` command,
or send the contents to `stdout` with the `git secret cat` command.
If you used a password on your GPG key ring,
it will ask you for your password.

## Step 5: Adding Someone to a Repository
Adding someone to a repository using `git-secret`, you need to do the following

1. Get their gpg public-key. You won’t need their secret key.
2. Import this key into your gpg setup (in `~/.gnupg` or similar) by running `gpg --import KEY_NAME.txt`.
3. Now add this person to your secrets repo by running `git secret tell persons@email.id`
(NOTE: This must be the email address associated with the public key)
4. The newly added user cannot yet read the encrypted files.
You must first re-encrypt the files using `git secret reveal`, then `git secret hide` or `git secret hide -d`,
and then commit and push the newly encrypted files.
(NOTE: The `-d` options deletes the unencrypted file after re-encrypting it).
Now the newly added user be able to decrypt the files in the repo using `git-secret`.



[01]:https://git-secret.io/
[02]:https://github.com/sobolevn/git-secret
[03]:https://git-secret.io/installation
