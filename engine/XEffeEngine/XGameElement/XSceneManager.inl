INLINE XScene * XSceneManager::getScene(const std::string &name)
{
	if(m_scenes.count(name) > 0)
		return m_scenes[name];
	return NULL;
}
