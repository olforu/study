set nocompatible              " be iMproved, required
filetype off                  " required
set mouse=a "alway use mouse
"set Tab=4 spaces
set ts=4
"highlight search things
set hlsearch
"enable outside clipboard
"set clipboard=unamed
set tags+=/home/duke/Downloads/mosquitto-1.3.1/tags

"//////////vim shortcut/////////////////
nmap <F4> :grep "" ./ -r -n
let mapleader="\<space>"
"nnoremap <Leader>co   :copen<CR>
nnoremap <Leader>w :w<CR>
nnoremap <Leader>q :q<CR>
nnoremap <Leader>c bye
cnoremap <C-p> <C-r>"
nnoremap <Leader>f :/
nnoremap <Leader>r :%s///g
map <Leader>y "+y<CR>
map <Leader>p "+p<CR>
map <F6> :copen<CR>
map <F9> :tabnew<CR>
map <F2> :bn<CR>
map <S-F2> :bp<CR>

" set the runtime path to include Vundle and initialize
set rtp+=~/.vim/bundle/Vundle.vim
call vundle#begin()
" alternatively, pass a path where Vundle should install plugins
"call vundle#begin('~/some/path/here')

" let Vundle manage Vundle, required
Plugin 'VundleVim/Vundle.vim'

" The following are examples of different formats supported.
" Keep Plugin commands between vundle#begin/end.
" plugin on GitHub repo
Plugin 'tpope/vim-fugitive'
" plugin from http://vim-scripts.org/vim/scripts.html
" Plugin 'L9'
" Git plugin not hosted on GitHub
Plugin 'git://git.wincent.com/command-t.git'
Plugin 'git://github.com/scrooloose/nerdtree.git'
Plugin 'git://github.com/Xuyuanp/nerdtree-git-plugin.git'
Plugin 'https://github.com/vim-scripts/taglist.vim'
Plugin 'scrooloose/syntastic'
Plugin 'Lokaltog/vim-powerline'

"Plugin 'Valloric/YouCompleteMe'

"hard to use ,remove ctrlp
"Plugin 'https://github.com/kien/ctrlp.vim' 

"
Plugin 'Yggdroot/LeaderF'


" git repos on your local machine (i.e. when working on your own plugin)
" Plugin 'file:///home/gmarik/path/to/plugin'
" The sparkup vim script is in a subdirectory of this repo called vim.
" Pass the path to set the runtimepath properly.
Plugin 'rstacruz/sparkup', {'rtp': 'vim/'}
" Install L9 and avoid a Naming conflict if you've already installed a
" different version somewhere else.
" Plugin 'ascenator/L9', {'name': 'newL9'}

" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
" To ignore plugin indent changes, instead use:
"filetype plugin on
"
" Brief help
" :PluginList       - lists configured plugins
" :PluginInstall    - installs plugins; append `!` to update or just :PluginUpdate
" :PluginSearch foo - searches for foo; append `!` to refresh local cache
" :PluginClean      - confirms removal of unused plugins; append `!` to auto-approve removal
"
" see :h vundle for more details or wiki for FAQ
" Put your non-Plugin stuff after this line
"
"//////// 设置NerdTree////////
map <F3> :NERDTreeMirror<CR>
map <F3> :NERDTreeToggle<CR>

"//////////Taglist setting//////////
let Tlist_Show_One_File=1     "不同时显示多个文件的tag，只显示当前文件的    
let Tlist_Exit_OnlyWindow=1   "如果taglist窗口是最后一个窗口，则退出vim   
let Tlist_Ctags_Cmd="/usr/bin/ctags" "将taglist与ctags关联 
let Tlist_Use_Right_Window=1
map <F8> :TlistToggle<CR>

"//////////Ctrlp setting//////////
"let g:ctrl_map = 'c-p'
"let g:ctrl_cmd = 'CtrlP'
"let g:ctrlp_working_path_mode=0
"let g:ctrlp_regexp=1
"let g:ctrlp_by_filename=1
"let g:ctrlp_mruf_max=500
"let g:ctrlp_max_files=200000
"let g:ctrlp_follow_symlinks=1
"let g:ctrlp_max_depth=10


"//////////LeaderF setting//////////
let g:Lf_CommandMap = {'<Tab>':['<Esc>']}
let g:Lf_ShortcutF = '<C-P>'
